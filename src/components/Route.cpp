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
