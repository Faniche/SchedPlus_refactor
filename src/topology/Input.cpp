//
// Created by faniche on 22-10-19.
//

#include "Input.h"

void Input::setNodesAndLinks() {
    vSetNodesAndLinks();
}

void Input::setStreams(uint32_t streamsNum) {
    vSetStreams(streamsNum);
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
        switch (pcp) {
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
        streamsGroupByPcp[pcp].push_back(id);
        getAllRoutes(stream, graph);
        streams.push_back(stream);
    }
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
