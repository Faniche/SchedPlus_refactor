//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_FULLDUPLEXLINK_H
#define SCHEDPLUS_REFACTOR_FULLDUPLEXLINK_H


#include <vector>
#include "DirectedLink.h"

class FullDuplexLink {
private:
    std::pair<std::shared_ptr<DirectedLink>, std::shared_ptr<DirectedLink>> links;
public:
    FullDuplexLink(std::shared_ptr<Node> &&_nodeA,
                   std::shared_ptr<Node> &&_nodeB,
                   std::shared_ptr<Port> &&_portA,
                   std::shared_ptr<Port> &&_portB,
                   uint32_t _macrotick);

    [[nodiscard]] const std::pair<std::shared_ptr<DirectedLink>, std::shared_ptr<DirectedLink>> &getLinks() const;
};


#endif //SCHEDPLUS_REFACTOR_FULLDUPLEXLINK_H
