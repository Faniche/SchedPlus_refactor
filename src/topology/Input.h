//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_INPUT_H
#define SCHEDPLUS_REFACTOR_INPUT_H


#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include "../components/node/Node.h"
#include "../components/link/DirectedLink.h"
#include "../components/stream/Stream.h"
#include "../../lib/json/json.hpp"
#include "../components/link/Graph.h"

class Input {
private:
    virtual void vSetNodesAndLinks() = 0;

    virtual void vSetStreams(uint32_t streamsNu) = 0;

public:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> esList;
    std::vector<std::shared_ptr<Node>> swList;
    std::map<link_id_t , std::shared_ptr<DirectedLink>> links;
    std::vector<std::shared_ptr<Stream>> streams;
    std::map<stream_id_t, int> streamsId;
    std::map<pcp_t , std::vector<stream_id_t>> streamsGroupByPcp;
    std::map<node_id_t, std::shared_ptr<Node>> nodeIdMap;
    std::map<std::string, std::shared_ptr<Node>> nodeNameMap;

    void setNodesAndLinks();

    void setStreams(uint32_t streamsNum);

    void setStreams(const std::string& streamFilePath);

    void getAllRoutes(std::shared_ptr<Stream> &stream, Graph &graph);
};


#endif //SCHEDPLUS_REFACTOR_INPUT_H
