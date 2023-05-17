#include "../include/graph.hpp"
#include <vector>

Graph::Graph(int n) {
    this->g.assign(n, std::vector<bool>(n, false));
    this->deg.assign(n, 0);
    this->m = 0;
}

Graph::Graph(const Graph &H) {
    int n = H.count_vertices();
    this->g.assign(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (H.g[i][j]) this->g[i][j] = true;
        }
    }
    this->deg.assign(n, 0);
    for (int i = 0; i < n; i++) {
        this->deg[i] = H.deg[i];
    }
    this->m = H.m;
}

void Graph::add_edge(int u, int v) {
    assert(u != v);
    if (this->g[u][v]) return;
    this->g[u][v] = this->g[v][u] = true;
    this->deg[u]++, this->deg[v]++;
    this->m++;
}

void Graph::remove_edge(int u, int v) {
    assert(u != v);
    if (!this->g[u][v]) return;
    this->g[u][v] = this->g[v][u] = false;
    this->deg[u]--, this->deg[v]--;
    this->m--;
}

int Graph::count_edges() const {
    return this->m;
}

int Graph::count_vertices() const {
    return this->g.size();
}

std::vector<int> Graph::neighborhood(int u) const {
    std::vector<int> answer;
    for (int i = 0; i < (int)g.size(); i++) {
        if (g[u][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<int> Graph::symmetric_difference(int u, int v) const {
    std::vector<int> answer;
    for (int i = 0; i < (int)g.size(); i++) {
        if (i != u && i != v && g[u][i] ^ g[v][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<Edge> Graph::edges() const {
    std::vector<Edge> answer;
    for (int i = 0; i < this->count_vertices(); i++) {
        for (int j = i + 1; j < this->count_vertices(); j++) {
            if (this->g[i][j])
                answer.emplace_back(i, j);
        }
    }

    return answer;
}

int Graph::degree(int v) const {
    return this->deg[v];
}

std::vector<int> Graph::degree() const {
    return this->deg;
}

bool Graph::is_connected() const {
    const Graph& g = *this;

    int n = g.count_vertices();
    std::vector<int> vis(n, 0);
    std::vector<int> stack;
    stack.push_back(0);

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (!vis[u]) {
            vis[u] = 1;
            for (auto v : g.neighborhood(u)) if (!vis[v])
                stack.push_back(v);
        }
    }

    for (auto u : vis) if (!u) 
        return false;
    return true;
}

std::vector<std::vector<int>> Graph::adjacency_list() const {
    int n = this->count_vertices();
    std::vector<std::vector<int>> adj(n);
    for (int u = 0; u < n; u++) {
        for (auto v : this->neighborhood(u))
            adj[u].push_back(v);
    }
    return adj;
}

int Graph::width(const ContractionSequence& seq) const {
    Graph H(this->count_vertices());
    int answer = 0;
    std::vector<bool> erased(this->count_vertices(), false);
    for (auto [p, u] : seq) {
        erased[u] = true;
        for (int x: H.neighborhood(u)) {
            H.remove_edge(u, x);
            if (x != p)
                H.add_edge(p, x);
        }
        for (int x: this->symmetric_difference(u, p)) {
            if (!erased[x])
                H.add_edge(p, x);
        }
        for (int i = 0; i < this->count_vertices(); i++)
            answer = std::max(answer, H.degree(i));
    }
    return answer;
}
