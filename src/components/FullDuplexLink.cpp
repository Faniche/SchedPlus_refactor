//
// Created by faniche on 22-10-19.
//

#include "FullDuplexLink.h"

FullDuplexLink::FullDuplexLink(std::shared_ptr<Node> &&_nodeA,
                               std::shared_ptr<Node> &&_nodeB,
                               std::shared_ptr<Port> &&_portA,
                               std::shared_ptr<Port> &&_portB,
                               uint32_t _macrotick) {
    links = std::make_pair(
            std::move(std::make_shared<DirectedLink>(_nodeA, _nodeB, _portA, _portB, _macrotick)),
            std::move(std::make_shared<DirectedLink>(_nodeB, _nodeA, _portB, _portA, _macrotick))
    );
}

const std::pair<std::shared_ptr<DirectedLink>, std::shared_ptr<DirectedLink>> &FullDuplexLink::getLinks() const {
    return links;
}
