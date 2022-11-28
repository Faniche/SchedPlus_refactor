//
// Created by faniche on 22-10-19.
//


#include <fstream>
#include "Line_2sw_2es.h"
#include "../components/link/Graph.h"

void Line_2sw_2es::vSetNodesAndLinks() {
    auto es0 = std::make_shared<EndSystem>("es0", 30);
    auto es1 = std::make_shared<EndSystem>("es1", 30);
    auto sw0 = std::make_shared<Switch>("sw0", 30);
    auto sw1 = std::make_shared<Switch>("sw1", 30);
    nodes.insert(nodes.begin(), {es0, es1, sw0, sw1});
    esList.insert(esList.begin(), {es0, es1});
    swList.insert(swList.begin(), {sw0, sw1});

    FullDuplexLink fdLink0(std::static_pointer_cast<Node>(es0),
                           std::static_pointer_cast<Node>(sw0),
                           es0->getPort(),
                           std::const_pointer_cast<Port>(sw0->getPorts()[0]),
                           _1000_ETHERNET);

    FullDuplexLink fdLink1(std::static_pointer_cast<Node>(es1),
                           std::static_pointer_cast<Node>(sw1),
                           es1->getPort(),
                           std::const_pointer_cast<Port>(sw1->getPorts()[0]),
                           _1000_ETHERNET);

    FullDuplexLink fdLink2(std::static_pointer_cast<Node>(sw0),
                           std::static_pointer_cast<Node>(sw1),
                           std::const_pointer_cast<Port>(sw0->getPorts()[1]),
                           std::const_pointer_cast<Port>(sw1->getPorts()[1]),
                           _1000_ETHERNET);
    std::vector<FullDuplexLink> fdLlinks{fdLink0, fdLink1, fdLink2};
    for (const auto& fdLink: fdLlinks) {
        for (const auto& dirLink: fdLink.getLinks()) {
            auto key = std::make_pair(dirLink->getSrcNode()->getId(), dirLink->getDestNode()->getId());
            links[key] = dirLink;
        }
    }

    for (int i = 0; i < nodes.size(); ++i) {
        nodeIdMap.emplace(i, nodes[i]);
        nodeNameMap.emplace(nodes[i]->getName(), nodes[i]);
    }
}

void Line_2sw_2es::vSetStreams(uint32_t streamsNum, std::string streamFilePath) {
    std::ifstream stream_file(streamFilePath);
    nlohmann::json jStreams;
    stream_file >> jStreams;
    Graph graph(nodes.size());
    graph.initGraph(nodeIdMap, links);
    for (auto & jStream : jStreams) {
        stream_id id            = jStream[  "id"  ].get<nlohmann::json::number_unsigned_t>();
        uint64_t period         = jStream["period"].get<nlohmann::json::number_unsigned_t>();
        uint64_t length         = jStream["length"].get<nlohmann::json::number_unsigned_t>();
        pcp_t pcp = jStream[ "pcp"  ].get<nlohmann::json::number_unsigned_t>();
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
        calAllRoutes(stream, graph);
        streams.push_back(stream);
    }
}
