//
// Created by faniche on 22-10-18.
//

#include "DirectedLink.h"

#include <utility>

DirectedLink::DirectedLink() {

}

DirectedLink::DirectedLink(std::shared_ptr<Node> _srcNode,
                           std::shared_ptr<Node> _destNode,
                           std::shared_ptr<Port> _srcPort,
                           std::shared_ptr<Port> _destPort) : srcNode(std::move(_srcNode)),
                                                  destNode(std::move(_destNode)),
                                                  srcPort(std::move(_srcPort)),
                                                  destPort(std::move(_destPort)) {
    id = std::make_pair(srcNode->getId(), destNode->getId());
}

DirectedLink::DirectedLink(std::shared_ptr<Node> _srcNode,
                           std::shared_ptr<Node> _destNode,
                           std::shared_ptr<Port> _srcPort,
                           std::shared_ptr<Port> _destPort,
                           uint32_t _len) : srcNode(std::move(_srcNode)),
                                                  destNode(std::move(_destNode)),
                                                  srcPort(std::move(_srcPort)),
                                                  destPort(std::move(_destPort)),
                                                  len(_len) {
    id = std::make_pair(srcNode->getId(), destNode->getId());
}

link_id_t DirectedLink::getId() const {
    return id;
}

void DirectedLink::setId(link_id_t _id) {
    id = _id;
}

std::shared_ptr<Node> DirectedLink::getSrcNode() const {
    return srcNode;
}

std::shared_ptr<Node> DirectedLink::getDestNode() const {
    return destNode;
}

std::shared_ptr<Port> DirectedLink::getSrcPort() const {
    return srcPort;
}

std::shared_ptr<Port> DirectedLink::getDestPort() const {
    return destPort;
}

void DirectedLink::addGateControlEntry(std::shared_ptr<GateControlEntry> &&gateControlEntry) {
    srcPort->addGateControlEntry(std::move(gateControlEntry));
}

void DirectedLink::clearGateControlEntry() {
    srcPort->clearGCL();
}

void DirectedLink::sortGCL() {
    srcPort->sortGCL();
}

void DirectedLink::mergeGCL() {
    srcPort->mergeGCL();
}

uint32_t DirectedLink::getLen() const {
    return len;
}

void DirectedLink::setLen(uint32_t _len) {
    len = _len;
}

uint32_t DirectedLink::getPropSpeed() const {
    return propSpeed;
}

void DirectedLink::setPropSpeed(uint32_t _propSpeed) {
    propSpeed = _propSpeed;
}
