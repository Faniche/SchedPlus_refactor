//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H
#define SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H


#include <vector>
#include "../../type.h"

class GateControlEntry {
private:
    std::vector<GATE_EVENT> gateStatesValue;
    uint64_t startTime;
    uint64_t timeIntervalValue;
public:
    GateControlEntry();

    [[nodiscard]] const std::vector<bool> &getGateStatesValue() const;

    void setGateStatesValue(uint8_t idx, GATE_EVENT gateState);

    [[nodiscard]] uint64_t getStartTime() const;

    void setStartTime(uint64_t _startTime);

    [[nodiscard]] uint64_t getTimeIntervalValue() const;

    void setTimeIntervalValue(uint64_t _timeIntervalValue);

    [[nodiscard]] std::string toBitVec() const;
};


#endif //SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H
