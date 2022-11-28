//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H
#define SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H


#include <vector>
#include "../../type.h"

class GateControlEntry {
private:
    std::vector<gate_event_t> gateStatesValue;
    uint64_t startTime;
    uint64_t timeIntervalValue;
public:
    GateControlEntry();

    [[nodiscard]] const std::vector<bool> &getGateStatesValue() const;

    void setGateStatesValue(uint8_t idx, gate_event_t gateState);

    [[nodiscard]] uint64_t getStartTime() const;

    void setStartTime(uint64_t _startTime);

    [[nodiscard]] uint64_t getTimeIntervalValue() const;

    void setTimeIntervalValue(uint64_t _timeIntervalValue);

    [[nodiscard]] std::string toBitVec() const;
};


#endif //SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H
