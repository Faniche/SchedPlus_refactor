//
// Created by faniche on 22-10-17.
//

#include "Port.h"


Port::Port(uint64_t _speed) {
    if (_speed != 1000000000)
        speed = _speed;
    id = g_port_counter++;
}

port_id_t Port::getId() const {
    return id;
}

uint64_t Port::getSpeed() const {
    return speed;
}

double Port::getTransmitSpd() const {
    return transmitSpd;
}

void Port::setTransmitSpd(double _transmitSpd) {
    Port::transmitSpd = _transmitSpd;
}

const std::vector<std::shared_ptr<GateControlEntry>> &Port::getGateControlList() const {
    return gateControlList;
}


void Port::addGateControlEntry(std::shared_ptr<GateControlEntry> &&gateControlEntry) {
    gateControlList.emplace_back(std::move(gateControlEntry));
}

void Port::clearGCL() {
    gateControlList.clear();
}

void Port::sortGCL() {
    std::sort(gateControlList.begin(), gateControlList.end(),
              [](const std::shared_ptr<GateControlEntry>& a, const std::shared_ptr<GateControlEntry>& b){
        return a->getStartTime() < b->getStartTime();
    });
}

void Port::mergeGCL() {
    for (auto iter = gateControlList.begin(); iter != gateControlList.end() - 1; ++iter) {
        if (iter->get()->getStartTime() + iter->get()->getTimeIntervalValue() + IFG_TIME >= (iter + 1)->get()->getStartTime()) {
            uint64_t queue_delay = iter->get()->getStartTime() + iter->get()->getTimeIntervalValue() - (iter + 1)->get()->getStartTime();
            uint64_t finish_time = (iter + 1)->get()->getStartTime() + (iter + 1)->get()->getTimeIntervalValue() + queue_delay;
            uint64_t time_interval = finish_time - iter->get()->getStartTime() + IFG_TIME;
            iter->get()->setTimeIntervalValue(time_interval);
            iter->get()->setGateStatesValue(5, GATE_OPEN);
            iter->get()->setGateStatesValue(6, GATE_OPEN);
            iter = gateControlList.erase(iter + 1);
        }
    }
}

uint64_t Port::getQsize() const {
    return qsize;
}

void Port::setQsize(uint64_t _qsize) {
    qsize = _qsize;
}

uint32_t Port::getQlen() const {
    return qlen;
}

void Port::setQlen(uint32_t _qlen) {
    qlen = _qlen;
}


