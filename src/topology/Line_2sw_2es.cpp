//
// Created by faniche on 22-10-19.
//


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
        nodeMap.emplace(std::make_pair(i, nodes[i]));
    }
}

void Line_2sw_2es::vSetFlows(uint32_t flowNum) {
    for (int i = 0; i < flowNum; ++i) {
        node_id a, b;
        if (i <= flowNum / 2) {
            a = 0;
            b = 1;
        } else {
            a = 1;
            b = 0;
        }
        auto stream = std::make_shared<Stream>(i, 1500, 1000, P5, nodeMap[a], nodeMap[b]);


    }
}
