//
// Created by faniche on 22-10-19.
//

#include "Input.h"

#include <utility>

void Input::setNodesAndLinks() {
    vSetNodesAndLinks();
}

void Input::setStreams(uint32_t streamsNum ,std::string streamFilePath) {
    vSetStreams(streamsNum, std::move(streamFilePath));
}
