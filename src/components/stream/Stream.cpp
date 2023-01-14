//
// Created by faniche on 22-10-17.
//

#include "Stream.h"
#include "../node/EndSystem.h"

Stream::Stream(stream_id_t id,
               sched_time_t period,
               uint32_t length,
               pcp_t pcp,
               std::shared_ptr<Node> &src,
               std::shared_ptr<Node> &dest) : id(id),
                                              period(period),
                                              length(length),
                                              pcp(pcp),
                                              src(src),
                                              dest(dest) {
    size_t frame_num = length / MTU + 1;
    if (frame_num == 1) {
        frames.push_back(std::make_shared<Frame>(std::make_pair(id, 0), 0, length, period));
    } else {
        for (frame_id_t i = 0; i < frame_num; ++i) {
            frames.push_back(std::make_shared<Frame>(
                std::make_pair(id, i), 0, i == frame_num - 1? length % MTU: MTU, period)
            );
        }
    }
    srcTransmitDelay = length * std::static_pointer_cast<EndSystem>(src)->getPort()->getTransmitSpd();
}

stream_id_t Stream::getId() const {
    return id;
}

void Stream::setId(stream_id_t _id) {
    id = _id;
}

sched_time_t Stream::getPeriod() const {
    return period;
}

void Stream::setPeriod(sched_time_t _period) {
    period = _period;
}

uint32_t Stream::getLength() const {
    return length;
}

void Stream::setLength(uint32_t _length) {
    length = _length;
}

pcp_t Stream::getPcp() const {
    return pcp;
}

void Stream::setPcp(pcp_t _pcp) {
    pcp = _pcp;
}

const DeliveryGuarantee *Stream::getDeliveryGuarantee() const {
    return deliveryGuarantee.get();
}

void Stream::setDeliveryGuarantee(std::unique_ptr<DeliveryGuarantee> &&_deliveryGuarantee) {
    deliveryGuarantee = std::move(_deliveryGuarantee);
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

void Stream::setRoutes(std::vector<std::shared_ptr<Route>> &&_routes) {
    routes = _routes;
}

const sched_time_t &Stream::getSrcTransmitDelay() const {
    return srcTransmitDelay;
}

uint32_t Stream::getChosenRoute() const {
    return chosenRoute;
}

void Stream::setChosenRoute(uint32_t selectedRouteInx) {
    chosenRoute = selectedRouteInx;
}

const std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> &Stream::getLinkFrames() const {
    return linkFrames;
}

void Stream::setFramesOffset(size_t idx, sched_time_t offset) {
    frames[idx]->setOffset(offset);
}
