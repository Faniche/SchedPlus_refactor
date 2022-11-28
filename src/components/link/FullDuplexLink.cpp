//
// Created by faniche on 22-10-19.
//

#include "FullDuplexLink.h"

FullDuplexLink::FullDuplexLink(std::shared_ptr<Node> &&_nodeA,
                               std::shared_ptr<Node> &&_nodeB,
                               std::shared_ptr<Port> &&_portA,
                               std::shared_ptr<Port> &&_portB,
                               eth_speed_t speed) {
    double portTransmitSpd = 0;
    switch (speed) {
        case _10_ETHERNET: {
            portTransmitSpd = 800;
            break;
        }
        case _100_ETHERNET: {
            portTransmitSpd = 80;
            break;
        }
        default: {
            portTransmitSpd = 8;
        }
    }
    _portA->setTransmitSpd(portTransmitSpd);
    _portB->setTransmitSpd(portTransmitSpd);
    links.emplace_back(
            std::move(std::make_shared<DirectedLink>(_nodeA, _nodeB, _portA, _portB))
            );
    links.emplace_back(
            std::move(std::make_shared<DirectedLink>(_nodeB, _nodeA, _portB, _portA))
            );
}

const std::vector<std::shared_ptr<DirectedLink>> &FullDuplexLink::getLinks() const {
    return links;
}