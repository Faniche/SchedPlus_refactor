//
// Created by faniche on 22-10-17.
//

#include "Frame.h"

Frame::Frame(std::pair<stream_id, frame_id> _id, uint64_t offset, uint32_t length, uint32_t period) : id(std::move(_id)),
                                                                                                            offset(offset),
                                                                                                            length(length),
                                                                                                            period(period) {}

uint64_t Frame::getOffset() const {
    return offset;
}

void Frame::setOffset(uint64_t offset) {
    Frame::offset = offset;
}

uint32_t Frame::getLength1() const {
    return length;
}

void Frame::setLength1(uint32_t length) {
    Frame::length = length;
}

uint32_t Frame::getPeriod1() const {
    return period;
}

void Frame::setPeriod1(uint32_t period) {
    Frame::period = period;
}

