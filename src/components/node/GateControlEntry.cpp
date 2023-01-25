//
// Created by faniche on 22-10-19.
//

#include "GateControlEntry.h"
GateControlEntry::GateControlEntry() {
    gateStatesValue.assign(8, GATE_OPEN);
    gateStatesValue[1] = GATE_CLOSE;
    gateStatesValue[2] = GATE_CLOSE;
    startTime = 0;
    timeIntervalValue = 0;
}

GateControlEntry::GateControlEntry(const std::vector<gate_event_t> &_gateStatesValue,
                                   sched_time_t _startTime,
                                   sched_time_t _timeIntervalValue) : gateStatesValue(_gateStatesValue), startTime(_startTime),
                                                                 timeIntervalValue(_timeIntervalValue) {}

GateControlEntry::GateControlEntry(const std::tuple<sched_time_t, sched_time_t, std::string> &gce) {
    gateStatesValue.assign(8, GATE_OPEN);
    startTime = std::get<0>(gce);
    timeIntervalValue = std::get<1>(gce) - startTime;
    for (size_t i = 0; i < 8; ++i) {
        gateStatesValue[i] = std::get<2>(gce)[i] == '1';
    }
}

const std::vector<bool> &GateControlEntry::getGateStatesValue() const {
    return gateStatesValue;
}

void GateControlEntry::setGateStatesValue(uint8_t idx, gate_event_t gateState) {
    gateStatesValue[idx] = gateState;
}

sched_time_t GateControlEntry::getStartTime() const {
    return startTime;
}

void GateControlEntry::setStartTime(sched_time_t _startTime) {
    startTime = _startTime;
}

sched_time_t GateControlEntry::getTimeIntervalValue() const {
    return timeIntervalValue;
}

void GateControlEntry::setTimeIntervalValue(sched_time_t _timeIntervalValue) {
    timeIntervalValue = _timeIntervalValue;
}

std::string GateControlEntry::toBitStr() const {
    std::string bitvector_str;
    for (auto const &gate_event: gateStatesValue) {
        if (gate_event == GATE_OPEN)
            bitvector_str += "1";
        else
            bitvector_str += "0";
    }
//    std::reverse(bitvector_str.begin(), bitvector_str.end());
    return bitvector_str;
}
