//
// Created by faniche on 22-10-19.
//

#include "Input.h"

void Input::setHyperPeriod() {
    for (const auto& stream: streams) {
        if (hyperPeriod != 0)
            hyperPeriod = std::lcm(hyperPeriod, stream->getPeriod());
        else
            hyperPeriod = stream->getPeriod();
    }
}

int Input::getRandInt(int min, int max) {
    std::random_device randomDevice;
    std::default_random_engine generator(randomDevice());
    std::uniform_int_distribution<int> randInteger;
    std::uniform_int_distribution<int>::param_type param(min, max);
    randInteger.param(param);
    return randInteger(generator);
}


void Input::setDeliveryGuarantee(std::shared_ptr<Stream> stream) {
    switch (stream->getPcp()) {
        case P7: {
            // TODO
        }
        case P6: {
            /* unit: ns*/
            stream->setDeliveryGuarantee(
                    std::make_unique<DeliveryGuarantee>(DDL, stream->getPeriod())
            );
            break;
        }
        case P5: {
            /* Typically less than 90% of period. */
            /* unit: ns*/
            stream->setDeliveryGuarantee(
                    std::make_unique<DeliveryGuarantee>(E2E, stream->getPeriod() / 10)
            );
            break;
        }
        case P4: {
            // TODO
        }
        case P3: {
            // TODO
        }
        case P2: {
            // TODO
        }
        case P1: {
            // TODO
        }
        default: {
            stream->setDeliveryGuarantee(
                    std::make_unique<DeliveryGuarantee>()
            );
        }
    }
}


void Input::saveStreams(const std::string &streamFilePath) {
    std::string file_path = streamFilePath;
//    file_path.append("streams.json");
    std::ofstream oss(file_path);
    nlohmann::json jStreams;
    for (const auto &stream: streams) {
        nlohmann::json jstream;
        jstream["id"] = stream->getId();
        jstream["period"] = stream->getPeriod();
        jstream["length"] = stream->getLength();
        jstream["pcp"] = stream->getPcp();
        jstream["src"] = stream->getSrc()->getName();
        jstream["dest"] = stream->getDest()->getName();
        jStreams.push_back(jstream);
    }
    oss << jStreams;
    oss.close();
}


void Input::setNodesAndLinks() {
    vSetNodesAndLinks();
}

void Input::setStreams(size_t streamsNum) {
    Graph graph(nodes.size());
    graph.initGraph(nodeIdMap, links);
    for (size_t i = 0; i < streamsNum; ++i) {
        stream_id_t streamId = i + 1;
        node_id_t src, dest;
        if (i < esList.size()) {
            src  = esList[i]->getId();
            do {
                dest = esList[getRandInt(0, esList.size() - 1)]->getId();
            } while (src == dest);
        } else {
            do {
                src  = esList[getRandInt(0, esList.size() - 1)]->getId();
                dest = esList[getRandInt(0, esList.size() - 1)]->getId();
            } while (src == dest);
        }
        pcp_t pcp = i < streamsNum * 0.75 ? P6 : P5;
        auto stream = std::make_shared<Stream>(
                streamId,
                Stream::getRandomPeriod(pcp),
                Stream::getRandomFrameLength(pcp),
                static_cast<pcp_t>(pcp),
                nodeIdMap[src],
                nodeIdMap[dest]
        );
        setDeliveryGuarantee(stream);
        streamsGroupByPcp[pcp].push_back(streamId);
        getAllRoutes(stream, graph);
        streams.push_back(stream);
    }

    for (int i = 0; i < streams.size(); ++i)
        streamsId[streams[i]->getId()] = i;
    setHyperPeriod();
}

void Input::setStreams(const std::string& streamFilePath) {
    std::ifstream stream_file(streamFilePath);
    nlohmann::json jStreams;
    stream_file >> jStreams;
    Graph graph(nodes.size());
    graph.initGraph(nodeIdMap, links);
    for (auto & jStream : jStreams) {
        stream_id_t id          = jStream[  "id"  ].get<nlohmann::json::number_unsigned_t>();
        stream_id_t period      = jStream["period"].get<nlohmann::json::number_unsigned_t>();
        uint64_t length         = jStream["length"].get<nlohmann::json::number_unsigned_t>();
        pcp_t pcp               = jStream[ "pcp"  ].get<nlohmann::json::number_unsigned_t>();
        std::string src         = jStream[ "src"  ].get<nlohmann::json::string_t>();
        std::string dest        = jStream[ "dest" ].get<nlohmann::json::string_t>();
        auto stream = std::make_shared<Stream>(
                id, period, length, static_cast<pcp_t>(pcp), nodeNameMap[src], nodeNameMap[dest]
        );
        setDeliveryGuarantee(stream);
        streamsGroupByPcp[pcp].push_back(id);
        getAllRoutes(stream, graph);
        streams.push_back(stream);
    }
    for (int i = 0; i < streams.size(); ++i)
        streamsId[streams[i]->getId()] = i;
    setHyperPeriod();
}

void Input::getAllRoutes(std::shared_ptr<Stream> &stream, Graph &graph) {
    node_id_t srcIdx = stream->getSrc()->getId();
    if (srcIdx == INT32_MAX)
        spdlog::get("console")->error("can not find the index of node: %s", stream->getSrc()->getName());
    node_id_t destIdx = stream->getDest()->getId();
    if (destIdx == INT32_MAX)
        spdlog::get("console")->error("can not find the index of node: %s", stream->getDest()->getName());
//    std::vector<std::shared_ptr<Route>> routes;
    std::vector<std::vector<node_id_t>> intgerRoutes;
    graph.getAllRoutes(srcIdx, destIdx, intgerRoutes);
    std::vector<std::shared_ptr<Route>> routes;
    for (auto &idxRoute: intgerRoutes) {
        srcIdx = idxRoute[0];
        auto route = std::make_shared<Route>();
        stream_id_t tmp = 0;
        for (int j = 1; j < idxRoute.size(); ++j) {
            destIdx = idxRoute[j];
            auto link = links[std::make_pair(srcIdx, destIdx)];
            if (link == nullptr) {
                spdlog::get("console")->error("Null pointer error.");
                exit(EXIT_FAILURE);
            }
            srcIdx = destIdx;
            route->addLink(link);
//                route.links.push_back(link1);
            /* Calculate the e2e latency except queue delay of stream. */
            tmp += (
                    (stream_id_t)(stream->getLength() * link->getSrcPort()->getTransmitSpd())
                    + link->getSrcNode()->getDpr()
                    + link->getLen() * link->getPropSpeed()
            );
        }
//        if (tmp > stream->getDeliveryGuarantee()->getLowerVal())
//            continue;
        route->setE2E(tmp);
        routes.push_back(route);
    }
    stream->setRoutes(std::move(routes));
}

int Input::getStreamPos(stream_id_t streamId) {
    return streamsId[streamId];
}

std::shared_ptr<Stream> Input::getStream(stream_id_t streamId) {
    return streams[streamsId[streamId]];
}

std::vector<std::shared_ptr<DirectedLink>> Input::getRouteLinks(stream_id_t streamId, route_t routeId) {
    return streams[streamsId[streamId]]->getRoutes()[routeId]->getLinks();
}

std::string Input::getLinkStr(link_id_t linkId) {
    std::string linkIdStr = "{";
    auto &link = links[linkId];
    linkIdStr.append(link->getSrcNode()->getName() + "[");
    linkIdStr.append(std::to_string(link->getSrcPort()->getId()) + "]");
    linkIdStr.append(" <--> ");
    linkIdStr.append(link->getDestNode()->getName() + "[");
    linkIdStr.append(std::to_string(link->getSrcPort()->getId()) + "]}");
    return linkIdStr;
}

