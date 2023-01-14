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
    sched_time_t startTime;
    sched_time_t timeIntervalValue;
public:
    GateControlEntry();

    GateControlEntry(const std::vector<gate_event_t> &_gateStatesValue,
                     sched_time_t _startTime,
                     sched_time_t _timeIntervalValue);

    explicit GateControlEntry(const std::tuple<sched_time_t, sched_time_t, std::string> &gce);

    [[nodiscard]] const std::vector<bool> &getGateStatesValue() const;

    void setGateStatesValue(uint8_t idx, gate_event_t gateState);

    [[nodiscard]] sched_time_t getStartTime() const;

    void setStartTime(sched_time_t _startTime);

    [[nodiscard]] sched_time_t getTimeIntervalValue() const;

    void setTimeIntervalValue(sched_time_t _timeIntervalValue);

    [[nodiscard]] std::string toBitStr() const;
};


#endif //SCHEDPLUS_REFACTOR_GATECONTROLENTRY_H
