//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_ROUTE_H
#define SCHEDPLUS_REFACTOR_ROUTE_H


#include <vector>
#include <memory>
#include "DirectedLink.h"

class Route {
private:
    std::vector<std::shared_ptr<DirectedLink>> links;

    /* End to end latency except queue delay */
    uint64_t e2e = 0;
public:
    [[nodiscard]] const std::vector<std::shared_ptr<DirectedLink>> &getLinks() const;

    void setLinks(const std::vector<std::shared_ptr<DirectedLink>> &_links);

    [[nodiscard]] uint64_t getE2E() const;

    void setE2E(uint64_t _e2e);

    void addLink(const std::shared_ptr<DirectedLink>& link);

    std::string toString();
};


#endif //SCHEDPLUS_REFACTOR_ROUTE_H
