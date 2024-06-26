//
// Created by faniche on 22-10-17.
//

#include "Switch.h"

const std::string &Switch::vToString() {
    return getName();
}

Switch::Switch(std::string &&_name, sched_time_t _dpr, size_t _portNum) : Node(std::move(_name), SWITCH, _dpr){
    g_port_counter = 0;
    for (int i = 0; i < _portNum; ++i) {
        ports.push_back(std::make_shared<Port>());
    }
}

const std::vector<std::shared_ptr<Port>> &Switch::getPorts() const {
    return ports;
}
