//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_INPUT_H
#define SCHEDPLUS_REFACTOR_INPUT_H


#include <vector>
#include <memory>
#include "../components/node/Node.h"
#include "../components/link/DirectedLink.h"
#include "../components/stream/Stream.h"
#include "../../lib/json/json.hpp"
#include "../components/link/Graph.h"

class Input {
private:
    virtual void vSetNodesAndLinks() = 0;

    virtual void vSetStreams(uint32_t streamsNum, std::string streamFilePath) = 0;

public:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> esList;
    std::vector<std::shared_ptr<Node>> swList;
    std::map<std::pair<node_id, node_id>, std::shared_ptr<DirectedLink>> links;
    std::vector<std::shared_ptr<Stream>> streams;
    std::map<node_id, std::shared_ptr<Node>> nodeIdMap;
    std::map<std::string, std::shared_ptr<Node>> nodeNameMap;

    void setNodesAndLinks();

    void setStreams(uint32_t streamsNum, std::string streamFilePath);

    void calAllRoutes(std::shared_ptr<Stream> &stream, Graph &graph);
};


#endif //SCHEDPLUS_REFACTOR_INPUT_H
