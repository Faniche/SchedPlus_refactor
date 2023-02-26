//
// Created by faniche on 23-1-23.
//

#ifndef SCHEDPLUS_REFACTOR_SNOW_7SW_18ES_H
#define SCHEDPLUS_REFACTOR_SNOW_7SW_18ES_H

#include "Input.h"
#include "../components/node/EndSystem.h"
#include "../components/node/Switch.h"
#include "../components/link/FullDuplexLink.h"

class Snow_7sw_18es: public Input {
private:
    void vSetNodesAndLinks() override;

    void vSetStreams(size_t streamsNum, std::string topology) override;
};


#endif //SCHEDPLUS_REFACTOR_SNOW_7SW_18ES_H
