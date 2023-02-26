//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H
#define SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H


#include "Input.h"
#include "../components/node/EndSystem.h"
#include "../components/node/Switch.h"
#include "../components/link/FullDuplexLink.h"

class Line_2sw_2es: public Input{
private:
    void vSetNodesAndLinks() override;

    void vSetStreams(size_t streamsNum, std::string topology) override;
};


#endif //SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H
