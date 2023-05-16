#include "../include/graph.hpp"

Graph::Graph(int n) {
    this->g.assign(n, std::vector<int>(n, 0));
}

void Graph::add_edge(int u, int v) {
    this->g[u][v] = this->g[v][u] = 1;
    this->m++;
}

int Graph::count_edges() {
    return this->m;
}

int Graph::count_vertices() {
    return this->g.size();
}

std::vector<int> Graph::neighborhood(int u) {
    std::vector<int> v;
    for (int i = 0; i < g.size(); i++) {
        if (g[u][i]) {
            v.push_back(i);
        }
    }
    return v;
}
