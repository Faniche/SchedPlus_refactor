//
// Created by faniche on 22-10-19.
//

#ifndef SCHEDPLUS_REFACTOR_INPUT_H
#define SCHEDPLUS_REFACTOR_INPUT_H


#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <random>
#include "../components/node/Node.h"
#include "../components/link/DirectedLink.h"
#include "../components/stream/Stream.h"
#include "../../lib/json/json.hpp"
#include "../components/link/Graph.h"

class Input {
private:
    virtual void vSetNodesAndLinks() = 0;

//    virtual void vSetStreams(uint32_t streamsNu) = 0;

    void setHyperPeriod();

    int getRandInt(int min, int max);

    void setDeliveryGuarantee(std::shared_ptr<Stream> stream);

public:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> esList;
    std::vector<std::shared_ptr<Node>> swList;
    std::map<link_id_t , std::shared_ptr<DirectedLink>> links;
    std::vector<std::shared_ptr<Stream>> streams;
    std::map<stream_id_t, size_t> streamsId;
    std::map<pcp_t , std::vector<stream_id_t>> streamsGroupByPcp;
    std::map<node_id_t, std::shared_ptr<Node>> nodeIdMap;
    std::map<std::string, std::shared_ptr<Node>> nodeNameMap;
    sched_time_t hyperPeriod = 0;
    std::vector<bool> optFlags;

    void saveStreams(const std::string& streamFilePath);

    void setNodesAndLinks();

    void setStreams(size_t streamsNum);

    void setStreams(const std::string& streamFilePath);

    void getAllRoutes(std::shared_ptr<Stream> &stream, Graph &graph);

    size_t getStreamPos(stream_id_t streamId);

    std::shared_ptr<Stream> getStream(stream_id_t streamId);

    std::vector<std::shared_ptr<DirectedLink>> getRouteLinks(stream_id_t streamId, route_t routeId);

    std::string getLinkStr(link_id_t linkId);

    void setOptFlags(std::vector<bool> &&_optFlags);
};


#endif //SCHEDPLUS_REFACTOR_INPUT_H
