//
// Created by faniche on 22-10-17.
//

#include "EndSystem.h"

const std::string &EndSystem::vToString() {
    return getName();
}

EndSystem::EndSystem(std::string &&_name, sched_time_t _dpr) : Node(std::move(_name), END_SYSTEM, _dpr) {
    g_port_counter = 0;
    port = std::make_shared<Port>();
}

std::shared_ptr<Port> EndSystem::getPort() const {
    return port;
}

