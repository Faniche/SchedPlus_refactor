//
// Created by faniche on 23-1-23.
//

#ifndef SCHEDPLUS_REFACTOR_RING_4SW_8ES_H
#define SCHEDPLUS_REFACTOR_RING_4SW_8ES_H


#include "Input.h"
#include "../components/node/EndSystem.h"
#include "../components/node/Switch.h"
#include "../components/link/FullDuplexLink.h"

class Ring_4sw_8es: public Input {
private:
    void vSetNodesAndLinks() override;
};



#endif //SCHEDPLUS_REFACTOR_RING_4SW_8ES_H
