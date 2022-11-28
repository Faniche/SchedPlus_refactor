//
// Created by faniche on 22-10-19.
//

#include "GateControlEntry.h"
GateControlEntry::GateControlEntry() {
    gateStatesValue.assign(8, GATE_OPEN);
    startTime = 0;
    timeIntervalValue = 0;
}

const std::vector<bool> &GateControlEntry::getGateStatesValue() const {
    return gateStatesValue;
}

void GateControlEntry::setGateStatesValue(uint8_t idx, gate_event_t gateState) {
    gateStatesValue[idx] = gateState;
}

uint64_t GateControlEntry::getStartTime() const {
    return startTime;
}

void GateControlEntry::setStartTime(uint64_t _startTime) {
    startTime = _startTime;
}

uint64_t GateControlEntry::getTimeIntervalValue() const {
    return timeIntervalValue;
}

void GateControlEntry::setTimeIntervalValue(uint64_t _timeIntervalValue) {
    timeIntervalValue = _timeIntervalValue;
}

std::string GateControlEntry::toBitVec() const {
    std::string bitvector_str;
    for (auto const &gate_event: gateStatesValue) {
        if (gate_event == GATE_OPEN)
            bitvector_str += "1";
        else
            bitvector_str += "0";
    }
    std::reverse(bitvector_str.begin(), bitvector_str.end());
    return bitvector_str;
}
