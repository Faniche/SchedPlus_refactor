//
// Created by faniche on 22-10-19.
//

#include "Input.h"

#include <utility>

void Input::setNodesAndLinks() {
    vSetNodesAndLinks();
}

void Input::setStreams(uint32_t streamsNum, std::string streamFilePath) {
    vSetStreams(streamsNum, std::move(streamFilePath));
}

void Input::calAllRoutes(std::shared_ptr<Stream> &stream, Graph &graph) {
    node_id srcIdx = stream->getSrc()->getId();
    if (srcIdx == INT32_MAX)
        spdlog::get("console")->error("can not find the index of node: %s", stream->getSrc()->getName());
    node_id destIdx = stream->getDest()->getId();
    if (destIdx == INT32_MAX)
        spdlog::get("console")->error("can not find the index of node: %s", stream->getDest()->getName());
//    std::vector<std::shared_ptr<Route>> routes;
    std::vector<std::vector<node_id>> intgerRoutes;
    graph.getAllRoutes(srcIdx, destIdx, intgerRoutes);
    std::vector<std::shared_ptr<Route>> routes;
    for (auto &idxRoute: intgerRoutes) {
        srcIdx = idxRoute[0];
        auto route = std::make_shared<Route>();
        uint64_t tmp = 0;
        for (int j = 1; j < idxRoute.size(); ++j) {
            destIdx = idxRoute[j];
            auto link = links[std::make_pair(srcIdx, destIdx)];
            if (link == nullptr) {
                spdlog::get("console")->error("Null pointer error.");
                exit(EXIT_FAILURE);
            }
            srcIdx = destIdx;
            route->addLink(link);
//                route.links.push_back(link1);
            /* Calculate the e2e latency except queue delay of stream. */
            uint64_t trans_delay = (uint64_t)(stream->getLength() * link->getSrcPort()->getTransmitSpd());
            uint64_t proc_delay = link->getSrcNode()->getDpr();
            uint64_t prop_delay = link->getLen() * link->getPropSpeed();
            tmp += trans_delay + proc_delay + prop_delay;
        }
//        if (tmp > stream->getDeliveryGuarantee()->getLowerVal())
//            continue;
        route->setE2E(tmp);
        routes.push_back(route);
    }
    stream->setRoutes(std::move(routes));
}
