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
    std::vector<int> answer;
    for (int i = 0; i < g.size(); i++) {
        if (g[u][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<int> Graph::symmetric_difference(int u, int v) {
	std::vector<int> answer;
    for (int i = 0; i < g.size(); i++) {
        if (i != u && i != v && g[u][i] ^ g[v][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}
