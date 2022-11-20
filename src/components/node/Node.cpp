//
// Created by faniche on 22-10-17.
//

#include "Node.h"

Node::Node(std::string &&_name, node_t _nodeType, uint64_t _dpr):
        name(std::move(_name)), nodeType(_nodeType), dpr(_dpr) {
    id = g_node_id++;
}

node_id Node::getId() const {
    return id;
}

void Node::setId(node_id _id) {
    id = _id;
}

const std::string &Node::getName() const {
    return name;
}

void Node::setName(std::string &&_name) {
    name = std::move(_name);
}

node_t Node::getNodeType() const {
    return nodeType;
}

const std::string & Node::toString() {
    return vToString();
}

uint64_t Node::getDpr() const {
    return dpr;
}

void Node::setDpr(uint64_t _dpr) {
    dpr = _dpr;
}

