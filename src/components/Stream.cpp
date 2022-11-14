//
// Created by faniche on 22-10-17.
//

#include "Stream.h"

Stream::Stream(stream_id id,
               uint64_t period,
               uint32_t length,
               PRIORITY_CODE_POINT pcp,
               std::shared_ptr<Node> &src,
               std::shared_ptr<Node> &dest) : id(id),
                                              period(period),
                                              length(length),
                                              pcp(pcp),
                                              src(src),
                                              dest(dest) {
    size_t frame_num = length / MTU + 1;
    if (frame_num == 1) {

    }
    for (int i = 0; i < frame_num; ++i) {
        uint32_t frame_len = length - i * MTU;
    }
}

stream_id Stream::getId() const {
    return id;
}

void Stream::setId(stream_id _id) {
    id = _id;
}

uint64_t Stream::getPeriod() const {
    return period;
}

void Stream::setPeriod(uint64_t _period) {
    period = _period;
}

uint32_t Stream::getLength() const {
    return length;
}

void Stream::setLength(uint32_t _length) {
    length = _length;
}

PRIORITY_CODE_POINT Stream::getPcp() const {
    return pcp;
}

void Stream::setPcp(PRIORITY_CODE_POINT _pcp) {
    pcp = _pcp;
}

const std::vector<DeliveryGuarantee> &Stream::getDeliveryGuarantees() const {
    return deliveryGuarantees;
}

void Stream::setDeliveryGuarantees(const std::vector<DeliveryGuarantee> &_deliveryGuarantees) {
    deliveryGuarantees = _deliveryGuarantees;
}

std::shared_ptr<Node> Stream::getSrc() const {
    return src;
}

void Stream::setSrc(std::shared_ptr<Node> &_src) {
    src = _src;
}

std::shared_ptr<Node> Stream::getDest() const {
    return dest;
}

void Stream::setDest(std::shared_ptr<Node> &_dest) {
    dest = _dest;
}

const std::vector<std::shared_ptr<Route>> &Stream::getRoutes() const {
    return routes;
}

void Stream::setRoutes(const std::vector<std::shared_ptr<Route>> &_routes) {
    routes = _routes;
}

uint32_t Stream::getSelectedRouteInx() const {
    return selectedRouteInx;
}

void Stream::setSelectedRouteInx(uint32_t _selectedRouteInx) {
    selectedRouteInx = _selectedRouteInx;
}

const std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> &Stream::getFrames() const {
    return frames;
}

void Stream::addFrames(int macrotick) {
    if (macrotick == 0)
        return;

    Stream::frames = frames;
}
