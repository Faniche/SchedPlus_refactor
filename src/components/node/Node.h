//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_NODE_H
#define SCHEDPLUS_REFACTOR_NODE_H

#include <cstdint>
#include <string>
#include "../../type.h"

extern int g_node_id;

enum node_t {
    SWITCH,
    END_SYSTEM
};

class Node {
private:
    node_id id;
    std::string name;
    node_t nodeType;
    /* the process delay of node: ns*/
    uint64_t dpr{};
    virtual const std::string& vToString() = 0;
public:
    Node(std::string &&_name, node_t _nodeType, uint64_t _dpr);

    virtual ~Node() = default;

    [[nodiscard]] node_id getId() const;

    void setId(node_id _id);

    [[nodiscard]] const std::string &getName() const;

    void setName(std::string&& _name);

    [[nodiscard]] node_t getNodeType() const;

    const std::string & toString();

    [[nodiscard]] uint64_t getDpr() const;

    void setDpr(uint64_t _dpr);
};


#endif //SCHEDPLUS_REFACTOR_NODE_H
