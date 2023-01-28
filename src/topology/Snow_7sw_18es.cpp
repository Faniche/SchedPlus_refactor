//
// Created by faniche on 23-1-23.
//

#include "Snow_7sw_18es.h"

void Snow_7sw_18es::vSetNodesAndLinks() {
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

    auto sw0 = std::make_shared<Switch>("switch0", 20000);
    auto sw1 = std::make_shared<Switch>("switch1", 20000);
    auto sw2 = std::make_shared<Switch>("switch2", 20000);
    auto sw3 = std::make_shared<Switch>("switch3", 20000);
    auto sw4 = std::make_shared<Switch>("switch4", 20000);
    auto sw5 = std::make_shared<Switch>("switch5", 20000);
    auto sw6 = std::make_shared<Switch>("switch6", 20000);
    nodes.insert(nodes.begin(), {es00, es01, es02, es03, es04, es05, es06,
                                 es07, es08, es09, es10, es11, es12, es13,
                                 es14, es15, es16, es17,
                                 sw0, sw1, sw2, sw3, sw4, sw5, sw6});
    esList.insert(esList.begin(), {es00, es01, es02, es03, es04, es05, es06,
                                   es07, es08, es09, es10, es11, es12, es13,
                                   es14, es15, es16, es17});
    swList.insert(swList.begin(), {sw0, sw1, sw2, sw3, sw4, sw5, sw6});

    FullDuplexLink fdLink00(std::static_pointer_cast<Node>(es00),
                            std::static_pointer_cast<Node>(sw0),
                            es00->getPort(),
                            std::const_pointer_cast<Port>(sw0->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink01(std::static_pointer_cast<Node>(es01),
                            std::static_pointer_cast<Node>(sw0),
                            es01->getPort(),
                            std::const_pointer_cast<Port>(sw0->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink02(std::static_pointer_cast<Node>(es02),
                            std::static_pointer_cast<Node>(sw0),
                            es02->getPort(),
                            std::const_pointer_cast<Port>(sw0->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink03(std::static_pointer_cast<Node>(es03),
                            std::static_pointer_cast<Node>(sw1),
                            es03->getPort(),
                            std::const_pointer_cast<Port>(sw1->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink04(std::static_pointer_cast<Node>(es04),
                            std::static_pointer_cast<Node>(sw1),
                            es04->getPort(),
                            std::const_pointer_cast<Port>(sw1->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink05(std::static_pointer_cast<Node>(es05),
                            std::static_pointer_cast<Node>(sw1),
                            es05->getPort(),
                            std::const_pointer_cast<Port>(sw1->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink06(std::static_pointer_cast<Node>(es06),
                            std::static_pointer_cast<Node>(sw2),
                            es06->getPort(),
                            std::const_pointer_cast<Port>(sw2->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink07(std::static_pointer_cast<Node>(es07),
                            std::static_pointer_cast<Node>(sw2),
                            es07->getPort(),
                            std::const_pointer_cast<Port>(sw2->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink08(std::static_pointer_cast<Node>(es08),
                            std::static_pointer_cast<Node>(sw2),
                            es08->getPort(),
                            std::const_pointer_cast<Port>(sw2->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink09(std::static_pointer_cast<Node>(es09),
                            std::static_pointer_cast<Node>(sw3),
                            es09->getPort(),
                            std::const_pointer_cast<Port>(sw3->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink10(std::static_pointer_cast<Node>(es10),
                            std::static_pointer_cast<Node>(sw3),
                            es10->getPort(),
                            std::const_pointer_cast<Port>(sw3->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink11(std::static_pointer_cast<Node>(es11),
                            std::static_pointer_cast<Node>(sw3),
                            es11->getPort(),
                            std::const_pointer_cast<Port>(sw3->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink12(std::static_pointer_cast<Node>(es12),
                            std::static_pointer_cast<Node>(sw4),
                            es12->getPort(),
                            std::const_pointer_cast<Port>(sw4->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink13(std::static_pointer_cast<Node>(es13),
                            std::static_pointer_cast<Node>(sw4),
                            es13->getPort(),
                            std::const_pointer_cast<Port>(sw4->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink14(std::static_pointer_cast<Node>(es14),
                            std::static_pointer_cast<Node>(sw4),
                            es14->getPort(),
                            std::const_pointer_cast<Port>(sw4->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink15(std::static_pointer_cast<Node>(es15),
                            std::static_pointer_cast<Node>(sw5),
                            es15->getPort(),
                            std::const_pointer_cast<Port>(sw5->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink16(std::static_pointer_cast<Node>(es16),
                            std::static_pointer_cast<Node>(sw5),
                            es16->getPort(),
                            std::const_pointer_cast<Port>(sw5->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink17(std::static_pointer_cast<Node>(es17),
                            std::static_pointer_cast<Node>(sw5),
                            es17->getPort(),
                            std::const_pointer_cast<Port>(sw5->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink18(std::static_pointer_cast<Node>(sw0),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw0->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[0]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink19(std::static_pointer_cast<Node>(sw1),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw1->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[1]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink20(std::static_pointer_cast<Node>(sw2),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw2->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[2]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink21(std::static_pointer_cast<Node>(sw3),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw3->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[3]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink22(std::static_pointer_cast<Node>(sw4),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw4->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[4]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink23(std::static_pointer_cast<Node>(sw5),
                            std::static_pointer_cast<Node>(sw6),
                            std::const_pointer_cast<Port>(sw5->getPorts()[3]),
                            std::const_pointer_cast<Port>(sw6->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);

    /* Links connected four switches */
    FullDuplexLink fdLink24(std::static_pointer_cast<Node>(sw0),
                            std::static_pointer_cast<Node>(sw1),
                            std::const_pointer_cast<Port>(sw0->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw1->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink25(std::static_pointer_cast<Node>(sw1),
                            std::static_pointer_cast<Node>(sw2),
                            std::const_pointer_cast<Port>(sw1->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw2->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink26(std::static_pointer_cast<Node>(sw2),
                            std::static_pointer_cast<Node>(sw3),
                            std::const_pointer_cast<Port>(sw2->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw3->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink27(std::static_pointer_cast<Node>(sw3),
                            std::static_pointer_cast<Node>(sw4),
                            std::const_pointer_cast<Port>(sw3->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw4->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink28(std::static_pointer_cast<Node>(sw4),
                            std::static_pointer_cast<Node>(sw5),
                            std::const_pointer_cast<Port>(sw4->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw5->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink29(std::static_pointer_cast<Node>(sw5),
                            std::static_pointer_cast<Node>(sw0),
                            std::const_pointer_cast<Port>(sw5->getPorts()[4]),
                            std::const_pointer_cast<Port>(sw0->getPorts()[5]),
                            SCHEDPLUS_1000_ETHERNET);


    std::vector<FullDuplexLink> fdLlinks{fdLink00, fdLink01, fdLink02, fdLink03, fdLink04, fdLink05,
                                         fdLink06, fdLink07, fdLink08, fdLink09, fdLink10, fdLink11,
                                         fdLink12, fdLink13, fdLink14, fdLink15, fdLink16, fdLink17,
                                         fdLink18, fdLink19, fdLink20, fdLink21, fdLink22, fdLink23,
                                         fdLink24, fdLink25, fdLink26, fdLink27, fdLink28, fdLink29};
    for (const auto &fdLink: fdLlinks) {
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
