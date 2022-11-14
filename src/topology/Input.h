//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_INPUT_H
#define SCHEDPLUS_REFACTOR_INPUT_H


#include <vector>
#include <memory>
#include "../components/Node.h"
#include "../components/DirectedLink.h"
#include "../components/Stream.h"

class Input {
private:
    virtual void vSetNodesAndLinks() = 0;
    virtual void vSetFlows(uint32_t flowNum) = 0;
public:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> esList;
    std::vector<std::shared_ptr<Node>> swList;
    std::vector<std::shared_ptr<DirectedLink>> links;
    std::vector<std::shared_ptr<Stream>> flows;
    std::map<node_id, std::shared_ptr<Node>> nodeMap;

    void setNodesAndLinks();

    void setFlows(uint32_t flowNum);
};


#endif //SCHEDPLUS_REFACTOR_INPUT_H
