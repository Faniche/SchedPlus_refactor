//
// Created by faniche on 23-1-23.
//

#include "Ring_4sw_8es.h"

void Ring_4sw_8es::vSetNodesAndLinks() {
    auto es0 = std::make_shared<EndSystem>("es0", 0);
    auto es1 = std::make_shared<EndSystem>("es1", 0);
    auto es2 = std::make_shared<EndSystem>("es2", 0);
    auto es3 = std::make_shared<EndSystem>("es3", 0);
    auto es4 = std::make_shared<EndSystem>("es4", 0);
    auto es5 = std::make_shared<EndSystem>("es5", 0);
    auto es6 = std::make_shared<EndSystem>("es6", 0);
    auto es7 = std::make_shared<EndSystem>("es7", 0);
    auto sw0 = std::make_shared<Switch>("switch0", 20000);
    auto sw1 = std::make_shared<Switch>("switch1", 20000);
    auto sw2 = std::make_shared<Switch>("switch2", 20000);
    auto sw3 = std::make_shared<Switch>("switch3", 20000);
    nodes.insert(nodes.begin(), {es0, es1, es2, es3, es4, es5, es6, es7, sw0, sw1, sw2, sw3});
    esList.insert(esList.begin(), {es0, es1, es2, es3, es4, es5, es6, es7});
    swList.insert(swList.begin(), {sw0, sw1, sw2, sw3});

    FullDuplexLink fdLink0(std::static_pointer_cast<Node>(es0),
                           std::static_pointer_cast<Node>(sw0),
                           es0->getPort(),
                           std::const_pointer_cast<Port>(sw0->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink1(std::static_pointer_cast<Node>(es1),
                           std::static_pointer_cast<Node>(sw0),
                           es1->getPort(),
                           std::const_pointer_cast<Port>(sw0->getPorts()[1]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink2(std::static_pointer_cast<Node>(es2),
                           std::static_pointer_cast<Node>(sw1),
                           es2->getPort(),
                           std::const_pointer_cast<Port>(sw1->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink3(std::static_pointer_cast<Node>(es3),
                           std::static_pointer_cast<Node>(sw1),
                           es3->getPort(),
                           std::const_pointer_cast<Port>(sw1->getPorts()[1]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink4(std::static_pointer_cast<Node>(es4),
                           std::static_pointer_cast<Node>(sw2),
                           es4->getPort(),
                           std::const_pointer_cast<Port>(sw2->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink5(std::static_pointer_cast<Node>(es5),
                           std::static_pointer_cast<Node>(sw2),
                           es5->getPort(),
                           std::const_pointer_cast<Port>(sw2->getPorts()[1]),
                           SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink6(std::static_pointer_cast<Node>(es6),
                           std::static_pointer_cast<Node>(sw3),
                           es6->getPort(),
                           std::const_pointer_cast<Port>(sw3->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink7(std::static_pointer_cast<Node>(es7),
                           std::static_pointer_cast<Node>(sw3),
                           es7->getPort(),
                           std::const_pointer_cast<Port>(sw3->getPorts()[1]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink8(std::static_pointer_cast<Node>(sw0),
                           std::static_pointer_cast<Node>(sw1),
                           std::const_pointer_cast<Port>(sw0->getPorts()[2]),
                           std::const_pointer_cast<Port>(sw1->getPorts()[3]),
                           SCHEDPLUS_1000_ETHERNET);
    FullDuplexLink fdLink9(std::static_pointer_cast<Node>(sw1),
                           std::static_pointer_cast<Node>(sw2),
                            std::const_pointer_cast<Port>(sw1->getPorts()[2]),
                            std::const_pointer_cast<Port>(sw2->getPorts()[3]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink10(std::static_pointer_cast<Node>(sw2),
                           std::static_pointer_cast<Node>(sw3),
                            std::const_pointer_cast<Port>(sw2->getPorts()[2]),
                            std::const_pointer_cast<Port>(sw3->getPorts()[3]),
                            SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink11(std::static_pointer_cast<Node>(sw3),
                           std::static_pointer_cast<Node>(sw0),
                           std::const_pointer_cast<Port>(sw3->getPorts()[2]),
                           std::const_pointer_cast<Port>(sw0->getPorts()[3]),
                           SCHEDPLUS_1000_ETHERNET);
    std::vector<FullDuplexLink> fdLlinks{fdLink0, fdLink1, fdLink2, fdLink3, fdLink4, fdLink5, fdLink6, fdLink7, fdLink8, fdLink9, fdLink10, fdLink11};
    for (const auto& fdLink: fdLlinks) {
        for (const auto& dirLink: fdLink.getLinks()) {
//            link_id_t linkId = std::make_pair(dirLink->getSrcNode()->getId(), dirLink->getDestNode()->getId());
            links[dirLink->getId()] = dirLink;
        }
    }

    for (auto & node : nodes) {
        nodeIdMap.emplace(node->getId(), node);
        nodeNameMap.emplace(node->getName(), node);
    }
}

void Ring_4sw_8es::vSetStreams(size_t streamsNum, std::string topology) {

}
