//
// Created by faniche on 22-10-17.
//

#include "Route.h"

const std::vector<std::shared_ptr<DirectedLink>> &Route::getLinks() const {
    return links;
}

void Route::setLinks(const std::vector<std::shared_ptr<DirectedLink>> &_links) {
    links = _links;
}

uint64_t Route::getE2E() const {
    return e2e;
}

void Route::setE2E(uint64_t _e2e) {
    e2e = _e2e;
}

void Route::addLink(const std::shared_ptr <DirectedLink>& link) {
    links.push_back(link);
}

std::string Route::toString() {
    std::string routeStr = links[0]->getSrcNode()->getName();
    for (size_t i = 1; i < links.size(); ++i)
        routeStr.append(" -> " + links[i]->getSrcNode()->getName());
    routeStr.append(" -> " + links[links.size() - 1]->getDestNode()->getName());
    return routeStr;
}
