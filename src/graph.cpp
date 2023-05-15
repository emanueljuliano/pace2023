#include <iostream>
#include "../include/graph.hpp"

Graph::Graph(int n) {
    this->g.assign(n, std::vector<int>(n, 0));
}

void Graph::add_edge(int u, int v) {
    this->g[u][v] = this->g[v][u] = 1;
}
