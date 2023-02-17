//
// Created by faniche on 23-1-23.
//

#include "Tree_7sw_21es.h"

void Tree_7sw_21es::vSetNodesAndLinks() {
    auto es00 = std::make_shared<EndSystem>("es00", 0);
    auto es01 = std::make_shared<EndSystem>("es01", 0);
    auto es02 = std::make_shared<EndSystem>("es02", 0);
    auto es03 = std::make_shared<EndSystem>("es03", 0);
    auto es04 = std::make_shared<EndSystem>("es04", 0);
    auto es05 = std::make_shared<EndSystem>("es05", 0);
    auto es06 = std::make_shared<EndSystem>("es06", 0);
    auto es07 = std::make_shared<EndSystem>("es07", 0);
    auto es08 = std::make_shared<EndSystem>("es08", 0);
    auto es09 = std::make_shared<EndSystem>("es09", 0);
    auto es10 = std::make_shared<EndSystem>("es10", 0);
    auto es11 = std::make_shared<EndSystem>("es11", 0);
    auto es12 = std::make_shared<EndSystem>("es12", 0);
    auto es13 = std::make_shared<EndSystem>("es13", 0);
    auto es14 = std::make_shared<EndSystem>("es14", 0);
    auto es15 = std::make_shared<EndSystem>("es15", 0);
    auto es16 = std::make_shared<EndSystem>("es16", 0);
    auto es17 = std::make_shared<EndSystem>("es17", 0);
    auto es18 = std::make_shared<EndSystem>("es18", 0);
    auto es19 = std::make_shared<EndSystem>("es19", 0);
    auto es20 = std::make_shared<EndSystem>("es20", 0);

    auto sw0 = std::make_shared<Switch>("switch0", 20000);
    auto sw1 = std::make_shared<Switch>("switch1", 20000);
    auto sw2 = std::make_shared<Switch>("switch2", 20000);
    auto sw3 = std::make_shared<Switch>("switch3", 20000);
    auto sw4 = std::make_shared<Switch>("switch4", 20000);
    auto sw5 = std::make_shared<Switch>("switch5", 20000);
    auto sw6 = std::make_shared<Switch>("switch6", 20000);
    nodes.insert(nodes.begin(), {es00, es01, es02, es03, es04, es05, es06,
                                           es07, es08, es09, es10, es11, es12, es13,
                                           es14, es15, es16, es17, es18, es19, es20,
                                           sw0, sw1, sw2, sw3, sw4, sw5, sw6});
    esList.insert(esList.begin(), {es00, es01, es02, es03, es04, es05, es06,
                                   es07, es08, es09, es10, es11, es12, es13,
                                   es14, es15, es16, es17, es18, es19, es20});
    swList.insert(swList.begin(), {sw0, sw1, sw2, sw3, sw4, sw5, sw6});

    FullDuplexLink fdLink00(std::static_pointer_cast<Node>(es00), std::static_pointer_cast<Node>(sw0), es00->getPort(), std::const_pointer_cast<Port>(sw0->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink01(std::static_pointer_cast<Node>(es01), std::static_pointer_cast<Node>(sw0), es01->getPort(), std::const_pointer_cast<Port>(sw0->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink02(std::static_pointer_cast<Node>(es02), std::static_pointer_cast<Node>(sw0), es02->getPort(), std::const_pointer_cast<Port>(sw0->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink03(std::static_pointer_cast<Node>(es03), std::static_pointer_cast<Node>(sw1), es03->getPort(), std::const_pointer_cast<Port>(sw1->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink04(std::static_pointer_cast<Node>(es04), std::static_pointer_cast<Node>(sw1), es04->getPort(), std::const_pointer_cast<Port>(sw1->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink05(std::static_pointer_cast<Node>(es05), std::static_pointer_cast<Node>(sw1), es05->getPort(), std::const_pointer_cast<Port>(sw1->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink06(std::static_pointer_cast<Node>(es06), std::static_pointer_cast<Node>(sw2), es06->getPort(), std::const_pointer_cast<Port>(sw2->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink07(std::static_pointer_cast<Node>(es07), std::static_pointer_cast<Node>(sw2), es07->getPort(), std::const_pointer_cast<Port>(sw2->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink08(std::static_pointer_cast<Node>(es08), std::static_pointer_cast<Node>(sw2), es08->getPort(), std::const_pointer_cast<Port>(sw2->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink09(std::static_pointer_cast<Node>(es09), std::static_pointer_cast<Node>(sw3), es09->getPort(), std::const_pointer_cast<Port>(sw3->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink10(std::static_pointer_cast<Node>(es10), std::static_pointer_cast<Node>(sw3), es10->getPort(), std::const_pointer_cast<Port>(sw3->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink11(std::static_pointer_cast<Node>(es11), std::static_pointer_cast<Node>(sw3), es11->getPort(), std::const_pointer_cast<Port>(sw3->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink12(std::static_pointer_cast<Node>(es12), std::static_pointer_cast<Node>(sw4), es12->getPort(), std::const_pointer_cast<Port>(sw4->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink13(std::static_pointer_cast<Node>(es13), std::static_pointer_cast<Node>(sw4), es13->getPort(), std::const_pointer_cast<Port>(sw4->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink14(std::static_pointer_cast<Node>(es14), std::static_pointer_cast<Node>(sw4), es14->getPort(), std::const_pointer_cast<Port>(sw4->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink15(std::static_pointer_cast<Node>(es15), std::static_pointer_cast<Node>(sw5), es15->getPort(), std::const_pointer_cast<Port>(sw5->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink16(std::static_pointer_cast<Node>(es16), std::static_pointer_cast<Node>(sw5), es16->getPort(), std::const_pointer_cast<Port>(sw5->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink17(std::static_pointer_cast<Node>(es17), std::static_pointer_cast<Node>(sw5), es17->getPort(), std::const_pointer_cast<Port>(sw5->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink18(std::static_pointer_cast<Node>(es18), std::static_pointer_cast<Node>(sw6), es18->getPort(), std::const_pointer_cast<Port>(sw6->getPorts()[0]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink19(std::static_pointer_cast<Node>(es19), std::static_pointer_cast<Node>(sw6), es19->getPort(), std::const_pointer_cast<Port>(sw6->getPorts()[1]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink20(std::static_pointer_cast<Node>(es20), std::static_pointer_cast<Node>(sw6), es20->getPort(), std::const_pointer_cast<Port>(sw6->getPorts()[2]), SCHEDPLUS_1000_ETHERNET);

    /* Links connected four switches */
    FullDuplexLink fdLink21(std::static_pointer_cast<Node>(sw0), std::static_pointer_cast<Node>(sw1), std::const_pointer_cast<Port>(sw0->getPorts()[3]), std::const_pointer_cast<Port>(sw1->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink22(std::static_pointer_cast<Node>(sw0), std::static_pointer_cast<Node>(sw2), std::const_pointer_cast<Port>(sw0->getPorts()[4]), std::const_pointer_cast<Port>(sw2->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink23(std::static_pointer_cast<Node>(sw1), std::static_pointer_cast<Node>(sw3), std::const_pointer_cast<Port>(sw1->getPorts()[4]), std::const_pointer_cast<Port>(sw3->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink24(std::static_pointer_cast<Node>(sw1), std::static_pointer_cast<Node>(sw4), std::const_pointer_cast<Port>(sw1->getPorts()[5]), std::const_pointer_cast<Port>(sw4->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink25(std::static_pointer_cast<Node>(sw2), std::static_pointer_cast<Node>(sw5), std::const_pointer_cast<Port>(sw2->getPorts()[4]), std::const_pointer_cast<Port>(sw5->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink26(std::static_pointer_cast<Node>(sw2), std::static_pointer_cast<Node>(sw6), std::const_pointer_cast<Port>(sw2->getPorts()[5]), std::const_pointer_cast<Port>(sw6->getPorts()[3]), SCHEDPLUS_1000_ETHERNET);


    std::vector<FullDuplexLink> fdLlinks{fdLink00, fdLink01, fdLink02, fdLink03, fdLink04, fdLink05,
                                        fdLink06, fdLink07, fdLink08, fdLink09, fdLink10, fdLink11,
                                        fdLink12, fdLink13, fdLink14, fdLink15, fdLink16, fdLink17,
                                        fdLink18, fdLink19, fdLink20, fdLink21, fdLink22, fdLink23,
                                        fdLink24, fdLink25, fdLink26};
    for (const auto& fdLink: fdLlinks) {
        for (auto dirLink: fdLink.getLinks()) {
            link_id_t linkId = std::make_pair(dirLink->getSrcNode()->getId(), dirLink->getDestNode()->getId());
            links[linkId] = dirLink;
        }
    }

    for (int i = 0; i < nodes.size(); ++i) {
        nodeIdMap.emplace(i, nodes[i]);
        nodeNameMap.emplace(nodes[i]->getName(), nodes[i]);
    }
}

void Tree_7sw_21es::vSetStreams() {
    Graph graph(nodes.size());
    graph.initGraph(nodeIdMap, links);
    stream_id_t streamId = 0;
    /* [es09 ~ es20] -> [es00 ~ es02]*/
    for (node_id_t src = nodeNameMap["es09"]->getId(); src <= nodeNameMap["es20"]->getId(); ++src) {
//        for (node_id_t dest = nodeNameMap["es00"]->getId(); dest <= nodeNameMap["es02"]->getId(); ++dest) {
//            sched_time_t period = 0;
//            uint32_t length = 0;
//            if (dest == 0) {
//                period = 10000000;
//                length = 800;
//            } else if (dest == 1) {
//                period = 10000000;
//                length = 800;
//            } else {
//                period = 10000000;
//                length = 800;
//            }
//            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[src], nodeIdMap[dest]);
//            finishStreamInit(graph, stream);
//        }
        for (node_id_t dest = nodeNameMap["es00"]->getId(); dest <= nodeNameMap["es02"]->getId(); ++dest) {
            sched_time_t period = 0;
            uint32_t length = 0;
            if (dest == 0) {
                period = 10000000;
                length = 800;
            } else if (dest == 1) {
                period = 10000000;
                length = 800;
            } else {
                period = 10000000;
                length = 800;
            }
            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[dest], nodeIdMap[src]);
            finishStreamInit(graph, stream);
        }
    }

    /* [es09 ~ es14] -> [es03 ~ es05]*/
    for (node_id_t src = nodeNameMap["es09"]->getId(); src <= nodeNameMap["es14"]->getId(); ++src) {
        for (node_id_t dest = nodeNameMap["es03"]->getId(); dest <= nodeNameMap["es05"]->getId(); ++dest) {
            sched_time_t period = 0;
            uint32_t length = 0;
            if (dest == 0) {
                period = 10000000;
                length = 800;
            } else if (dest == 1) {
                period = 10000000;
                length = 800;
            } else {
                period = 10000000;
                length = 800;
            }
            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[src], nodeIdMap[dest]);
            finishStreamInit(graph, stream);
        }
//        for (node_id_t dest = nodeNameMap["es03"]->getId(); dest <= nodeNameMap["es05"]->getId(); ++dest) {
//            sched_time_t period = 0;
//            uint32_t length = 0;
//            if (dest == 0) {
//                period = 10000000;
//                length = 800;
//            } else if (dest == 1) {
//                period = 10000000;
//                length = 800;
//            } else {
//                period = 10000000;
//                length = 800;
//            }
//            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[dest], nodeIdMap[src]);
//            finishStreamInit(graph, stream);
//        }
    }

    /* [es15 ~ es20] -> [es06 ~ es08]*/
    for (node_id_t src = nodeNameMap["es15"]->getId(); src <= nodeNameMap["es20"]->getId(); ++src) {
        for (node_id_t dest = nodeNameMap["es06"]->getId(); dest <= nodeNameMap["es08"]->getId(); ++dest) {
            sched_time_t period = 0;
            uint32_t length = 0;
            if (dest == 0) {
                period = 10000000;
                length = 800;
            } else if (dest == 1) {
                period = 10000000;
                length = 800;
            } else {
                period = 10000000;
                length = 800;
            }
            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[src], nodeIdMap[dest]);
            finishStreamInit(graph, stream);
        }
//        for (node_id_t dest = nodeNameMap["es06"]->getId(); dest <= nodeNameMap["es08"]->getId(); ++dest) {
//            sched_time_t period = 0;
//            uint32_t length = 0;
//            if (dest == 0) {
//                period = 10000000;
//                length = 800;
//            } else if (dest == 1) {
//                period = 10000000;
//                length = 800;
//            } else {
//                period = 10000000;
//                length = 800;
//            }
//            auto stream = std::make_shared<Stream>(++streamId, period, length, P5, nodeIdMap[dest], nodeIdMap[src]);
//            finishStreamInit(graph, stream);
//        }
    }

    /* [es09 ~ es20] -> [es09 ~ es20]*/
    for (node_id_t src = nodeNameMap["es09"]->getId(); src <= nodeNameMap["es20"]->getId(); ++src) {
        for (int i = 0; i < 2; ++i) {
            if (src % 3 == 0) {
                node_id_t dest = src + 1;
                auto stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
                dest = src + 2;
                stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
            } else if (src % 3 == 1) {
                node_id_t dest = src - 1;
                auto stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
                dest = src + 1;
                stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
            } else {
                node_id_t dest = src - 1;
                auto stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
                dest = src - 2;
                stream = std::make_shared<Stream>(
                        ++streamId,
                        500000,
                        80,
                        P6, nodeIdMap[src], nodeIdMap[dest]);
                finishStreamInit(graph, stream);
            }
        }
    }

    for (int i = 0; i < streams.size(); ++i)
        streamsId[streams[i]->getId()] = i;
    setHyperPeriod();
}

void Tree_7sw_21es::finishStreamInit(Graph &graph, std::shared_ptr<Stream>& stream) {
    setDeliveryGuarantee(stream);
    streamsGroupByPcp[stream->getPcp()].push_back(stream->getId());
    getAllRoutes(stream, graph);
    streams.push_back(stream);
}