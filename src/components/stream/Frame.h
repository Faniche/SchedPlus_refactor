//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_FRAME_H
#define SCHEDPLUS_REFACTOR_FRAME_H


#include <utility>
#include "../../type.h"



class Frame {
private:
    std::pair<stream_id_t, frame_id_t> id;
    sched_time_t offset;
    uint32_t length;
    sched_time_t period;
public:
    Frame(std::pair<stream_id_t, frame_id_t> _id,
          sched_time_t offset,
          uint32_t length,
          sched_time_t period);

    [[nodiscard]] sched_time_t getOffset() const;

    void setOffset(sched_time_t offset);

    [[nodiscard]] uint32_t getLength1() const;

    void setLength1(uint32_t length);

    [[nodiscard]] sched_time_t getPeriod1() const;

    void setPeriod1(sched_time_t period);
};


#endif //SCHEDPLUS_REFACTOR_FRAME_H
