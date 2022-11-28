//
// Created by faniche on 22-11-27.
//

#include "Graph.h"
// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void Graph::printAllPathsUtil(size_t u, size_t d,
                              std::vector<bool> &visited,
                              std::vector<node_id> &path,
                              node_id &path_index,
                              std::vector<std::vector<node_id>> &routes) {
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
    path_index++;

    /* If current vertex is same as destination, then store current path[] */
    if (u == d) {
        std::vector<node_id> tmp;
        for (size_t i = 0; i < path_index; i++)
            tmp.push_back(path[i]);
        routes.push_back(tmp);
    } else {
        /* If current vertex is not destination */
        /* Recur for all the vertices adjacent to current vertex */
        std::vector<node_id>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (!visited[*i])
                printAllPathsUtil(*i, d, visited, path, path_index, routes);
    }
    /* Remove current vertex from path[] and mark it as unvisited */
    path_index--;
    visited[u] = false;
}

Graph::Graph(size_t V) {
    this->V = V;
    for (node_id i = 0; i < V; ++i) {
        std::vector<node_id> tmp{i};
        adj.push_back(tmp);
    }
}

void Graph::addEdge(node_id u, node_id v) {
    adj[u].push_back(v); // Add v to u’s list.
}

// Prints all paths from 's' to 'd'
void Graph::getAllRoutes(node_id s, node_id d, std::vector<std::vector<node_id>> &routes) {
    // Initialize all vertices as not visited
    std::vector<bool> visited(V, false);

    // Create an array to store paths
    std::vector<node_id> path(V);

    node_id path_index = 0;

    // Call the recursive helper function to print all paths
    printAllPathsUtil(s, d, visited, path, path_index, routes);
}

/**
 * @brief Init the Integer type of graph
 * @param map       : reference to node index map
 * @param links     : link vector
 **/
void Graph::initGraph(const std::map<node_id, std::shared_ptr<Node>>& nodeIdMap,
                      const std::map<std::pair<node_id, node_id>, std::shared_ptr<DirectedLink>>& links) {
    for (auto &[key, _]: links) {
        addEdge(key.first, key.second);
    }
}