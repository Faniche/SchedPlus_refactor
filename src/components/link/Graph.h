//
// Created by faniche on 22-11-27.
//

#ifndef SCHEDPLUS_REFACTOR_GRAPH_H
#define SCHEDPLUS_REFACTOR_GRAPH_H

#include <map>
#include "../../type.h"
#include "../node/Node.h"
#include "DirectedLink.h"

// A directed graph using
// adjacency list representation
class Graph {
private:
    size_t V; // No. of vertices in graph
    std::vector<std::vector<node_id_t>> adj; // Pointer to an array containing adjacency lists

    // A recursive function used by getAllRoutes()
    void printAllPathsUtil(size_t, size_t, std::vector<bool> &, std::vector<node_id_t> &, node_id_t &,
                           std::vector<std::vector<node_id_t>> &routes);

public:
    explicit Graph(size_t V); // Constructor
    void addEdge(node_id_t u, node_id_t v);

    void getAllRoutes(node_id_t s, node_id_t d, std::vector<std::vector<node_id_t>> &routes);

    void initGraph(const std::map<node_id_t, std::shared_ptr<Node>>& nodeIdMap,
                   const std::map<std::pair<node_id_t, node_id_t>, std::shared_ptr<DirectedLink>>& links);
};



#endif //SCHEDPLUS_REFACTOR_GRAPH_H
