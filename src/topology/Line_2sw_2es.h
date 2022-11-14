//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H
#define SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H


#include "Input.h"
#include "../components/EndSystem.h"
#include "../components/Switch.h"
#include "../components/FullDuplexLink.h"

class Line_2sw_2es: public Input{
private:
    void vSetNodesAndLinks() override;

    void vSetFlows(uint32_t flowNum) override;
};


#endif //SCHEDPLUS_REFACTOR_LINE_2SW_2ES_H
