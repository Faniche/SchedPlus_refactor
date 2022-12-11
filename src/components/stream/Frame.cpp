//
// Created by faniche on 22-10-17.
//

#include "Frame.h"

Frame::Frame(std::pair<stream_id_t, frame_id_t> _id,
             sched_time_t offset,
             uint32_t length,
             sched_time_t period) : id(std::move(_id)),
                                    offset(offset),
                                    length(length),
                                    period(period) {}

sched_time_t Frame::getOffset() const {
    return offset;
}

void Frame::setOffset(sched_time_t offset) {
    Frame::offset = offset;
}

uint32_t Frame::getLength1() const {
    return length;
}

void Frame::setLength1(uint32_t length) {
    Frame::length = length;
}

sched_time_t Frame::getPeriod1() const {
    return period;
}

void Frame::setPeriod1(sched_time_t period) {
    Frame::period = period;
}

