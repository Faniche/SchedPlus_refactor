//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_FRAME_H
#define SCHEDPLUS_REFACTOR_FRAME_H


#include <utility>
#include "../../type.h"



class Frame {
private:
    std::pair<stream_id, frame_id> id;
    uint64_t offset;
    uint32_t length;
    uint32_t period;
public:
    Frame(std::pair<stream_id, frame_id> _id, uint64_t offset, uint32_t length, uint32_t period);

    [[nodiscard]] uint64_t getOffset() const;

    void setOffset(uint64_t offset);

    [[nodiscard]] uint32_t getLength1() const;

    void setLength1(uint32_t length);

    [[nodiscard]] uint32_t getPeriod1() const;

    void setPeriod1(uint32_t period);
};


#endif //SCHEDPLUS_REFACTOR_FRAME_H
