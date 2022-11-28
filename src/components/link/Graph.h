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
    std::vector<std::vector<node_id>> adj; // Pointer to an array containing adjacency lists

    // A recursive function used by getAllRoutes()
    void printAllPathsUtil(size_t, size_t, std::vector<bool> &, std::vector<node_id> &, node_id &,
                           std::vector<std::vector<node_id>> &routes);

public:
    explicit Graph(size_t V); // Constructor
    void addEdge(node_id u, node_id v);

    void getAllRoutes(node_id s, node_id d, std::vector<std::vector<node_id>> &routes);

    void initGraph(const std::map<node_id, std::shared_ptr<Node>>& nodeIdMap,
                   const std::map<std::pair<node_id, node_id>, std::shared_ptr<DirectedLink>>& links);
};



#endif //SCHEDPLUS_REFACTOR_GRAPH_H
