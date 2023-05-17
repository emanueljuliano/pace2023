#include "../include/solver.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <set>
#include <iostream>

Solver::Solver(Graph& G) : G(G) {
    assert(G.is_connected());
}

int Solver::tree_verifier() {
    int n = this->G.count_vertices(); assert(n > 1);
    int m = this->G.count_edges();

    if (m == n - 1) {
        std::vector<int> dist(n, -1);
        std::vector<int> stack;
        stack.push_back(0);
        dist[0] = 0;

        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            for (auto v : this->G.neighborhood(u)) if (dist[v] == -1) {
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
            for (auto v : this->G.neighborhood(u)) if (dist[v] == -1) {
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
            for (auto v : this->G.neighborhood(u)) {
                caterpillar[v] = 1;
            }
        }

        this->tree_contractor(root, root);
        for (auto u : caterpillar) if (!u)
            return 2;
        return 1;
    }
    return -1;
}

void Solver::tree_contractor(int root, int par) {
    auto nb = this->G.neighborhood(root);

    if (std::find(nb.begin(), nb.end(), par) != nb.end())
        nb.erase(std::find(nb.begin(), nb.end(), par));
    if (nb.empty())
        return;

    this->tree_contractor(nb[0], root);
    for (int i = 1; i < nb.size(); i++) {
        this->tree_contractor(nb[i], root);
        this->cs.emplace_back(nb[0], nb[i]);
    }

    this->cs.emplace_back(root, nb[0]);
}

void Solver::solve() {
    std::cout << "Twin-Width: " << tree_verifier() << std::endl;
}

void Solver::print_contraction() {
    for (auto [u, p] : this->cs)
        std::cout << u << " " << p << std::endl;
}

ContractionSequence Solver::get_contraction() {
    return this->cs;
}
