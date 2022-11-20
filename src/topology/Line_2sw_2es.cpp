//
// Created by faniche on 22-10-19.
//


#include <fstream>
#include "Line_2sw_2es.h"

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
                           1000);

    FullDuplexLink fdLink1(std::static_pointer_cast<Node>(es1),
                           std::static_pointer_cast<Node>(sw1),
                           es1->getPort(),
                           std::const_pointer_cast<Port>(sw1->getPorts()[0]),
                           1000);

    FullDuplexLink fdLink2(std::static_pointer_cast<Node>(sw0),
                           std::static_pointer_cast<Node>(sw1),
                           std::const_pointer_cast<Port>(sw0->getPorts()[1]),
                           std::const_pointer_cast<Port>(sw1->getPorts()[1]),
                           1000);

    links.insert(links.begin(), {
        fdLink0.getLinks().first, fdLink0.getLinks().second,
        fdLink1.getLinks().first, fdLink1.getLinks().second,
        fdLink2.getLinks().first, fdLink2.getLinks().second
    });

    for (int i = 0; i < nodes.size(); ++i) {
        nodeIdMap.emplace(std::make_pair(i, nodes[i]));
        nodeNameMap.emplace(std::make_pair(nodes[i]->getName(), nodes[i]));
    }
}

void Line_2sw_2es::vSetStreams(uint32_t streamsNum, std::string streamFilePath) {
    std::ifstream stream_file(streamFilePath);
    nlohmann::json jStreams;
    stream_file >> jStreams;
    for (auto & jStream : jStreams) {
        stream_id id            = jStream[  "id"  ].get<nlohmann::json::number_unsigned_t>();
        uint64_t period         = jStream["period"].get<nlohmann::json::number_unsigned_t>();
        uint64_t length         = jStream["length"].get<nlohmann::json::number_unsigned_t>();
        PRIORITY_CODE_POINT pcp = jStream[ "pcp"  ].get<nlohmann::json::number_unsigned_t>();
        std::string src         = jStream[ "src"  ].get<nlohmann::json::string_t>();
        std::string dest        = jStream[ "dest" ].get<nlohmann::json::string_t>();
        auto stream = std::make_shared<Stream>(
            id, period, length, static_cast<PRIORITY_CODE_POINT>(pcp), nodeNameMap[src], nodeNameMap[dest]
        );
        streams.push_back(stream);
        if (pcp == P6) {
            /* unit: ns*/
            stream->setDeliveryGuarantee(
                std::make_unique<DeliveryGuarantee>(DDL, stream->getPeriod())
            );
        } else if (pcp == P5) {
            /* Typically less than 90% of period. */
            /* unit: ns*/
            stream->setDeliveryGuarantee(
                std::make_unique<DeliveryGuarantee>(E2E, stream->getPeriod() / 10)
            );
        } else if (pcp == P5) {

        }
//        Util::calAllRoutes(nodeIdMap, flow, graph, links);
//        flows.push_back(flow);
//        oss.str("");
//        flow.toString(oss);
//        spdlog::get("console")->info("flow_{}: {}", flow.getId(), oss.str());
//

//    for (int i = 0; i < streamsNum; ++i) {
//        node_id a, b;
//        if (i <= streamsNum / 2) {
//            a = 0;
//            b = 1;
//        } else {
//            a = 1;
//            b = 0;
//        }
//        auto stream = std::make_shared<Stream>(i, 1500, 1000, P5, nodeIdMap[a], nodeIdMap[b]);
//
//
    }
}
