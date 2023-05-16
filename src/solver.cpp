#include "../include/solver.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <iostream>

bool is_connected(Graph g) {
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

Solver::Solver(Graph& g) : g(g) {
    assert(is_connected(g));
}

int Solver::tree_verifier() {
    int n = this->g.count_vertices(); assert(n > 1);
    int m = this->g.count_edges();

    if (m == n - 1) {
        std::vector<int> dist(n, -1);
        std::vector<int> stack;
        stack.push_back(0);
        dist[0] = 0;

        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            for (auto v : g.neighborhood(u)) if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                stack.push_back(v);
            }
        }
        
        int root = std::distance(
            dist.begin(),
            std::max_element(dist.begin(), dist.end())
        );

        dist.assign(n, -1);
        std::vector<int> par(n, -1);
        stack.push_back(root);
        dist[root] = 0;

        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            for (auto v : g.neighborhood(u)) if (dist[v] == -1) {
                par[v] = u;
                dist[v] = dist[u] + 1;
                stack.push_back(v);
            }
        }

        int end = std::distance(
            dist.begin(),
            std::max_element(dist.begin(), dist.end())
        );

        std::vector<int> diameter;
        while(par[end] != -1) {
            diameter.push_back(end);
            end = par[end];
        }
        diameter.push_back(end);

        std::vector<int> caterpillar(n, 0);
        for (auto u : diameter) {
            for (auto v : g.neighborhood(u)) {
                caterpillar[v] = 1;
            }
        }

        for (auto u : caterpillar) if (!u)
            return 2;
        return 1;
    }
    return -1;
}

void Solver::solve() {
    std::cout << tree_verifier() << std::endl;
}
