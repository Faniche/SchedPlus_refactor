//
// Created by faniche on 22-10-17.
//

#include "EndSystem.h"

const std::string &EndSystem::vToString() {
    return getName();
}

EndSystem::EndSystem(std::string &&_name, uint64_t _dpr) : Node(std::move(_name), END_SYSTEM, _dpr) {
    port = std::make_shared<Port>();
}

std::shared_ptr<Port> EndSystem::getPort() const {
    return port;
}

