//
// Created by faniche on 22-10-19.
//


#include "Line_2sw_2es.h"

void Line_2sw_2es::vSetNodesAndLinks() {
    auto es0 = std::make_shared<EndSystem>("es0", 0);
    auto es1 = std::make_shared<EndSystem>("es1", 0);
    auto sw0 = std::make_shared<Switch>("switch0", 20000);
    auto sw1 = std::make_shared<Switch>("switch1", 20000);
    nodes.insert(nodes.begin(), {es0, es1, sw0, sw1});
    esList.insert(esList.begin(), {es0, es1});
    swList.insert(swList.begin(), {sw0, sw1});

    FullDuplexLink fdLink0(std::static_pointer_cast<Node>(es0),
                           std::static_pointer_cast<Node>(sw0),
                           es0->getPort(),
                           std::const_pointer_cast<Port>(sw0->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink1(std::static_pointer_cast<Node>(es1),
                           std::static_pointer_cast<Node>(sw1),
                           es1->getPort(),
                           std::const_pointer_cast<Port>(sw1->getPorts()[0]),
                           SCHEDPLUS_1000_ETHERNET);

    FullDuplexLink fdLink2(std::static_pointer_cast<Node>(sw0),
                           std::static_pointer_cast<Node>(sw1),
                           std::const_pointer_cast<Port>(sw0->getPorts()[1]),
                           std::const_pointer_cast<Port>(sw1->getPorts()[1]),
                           SCHEDPLUS_1000_ETHERNET);
    std::vector<FullDuplexLink> fdLlinks{fdLink0, fdLink1, fdLink2};
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

void Line_2sw_2es::vSetStreams(size_t streamsNum, std::string topology) {

}
