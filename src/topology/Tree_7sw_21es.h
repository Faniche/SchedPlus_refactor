//
// Created by faniche on 23-1-23.
//

#ifndef SCHEDPLUS_REFACTOR_TREE_7SW_21ES_H
#define SCHEDPLUS_REFACTOR_TREE_7SW_21ES_H

#include "Input.h"
#include "../components/node/EndSystem.h"
#include "../components/node/Switch.h"
#include "../components/link/FullDuplexLink.h"

class Tree_7sw_21es: public Input {
private:
    void vSetNodesAndLinks() override;

    void vSetStreams(size_t streamsNum, std::string topology) override;

};


#endif //SCHEDPLUS_REFACTOR_TREE_7SW_21ES_H
