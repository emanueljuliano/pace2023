#include "../include/solver.hpp"
#include "../include/sat_solver.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <iostream>

Solver::Solver(Graph& G) : G(G) {
    assert(G.is_connected());
}

ContractionSequence Solver::solve_tree() {
    int n = this->G.count_vertices(); assert(n > 1);

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

    this->tree_contractor(root, -1);
    return cs;
}

void Solver::tree_contractor(int root, int par) {
    auto nb = this->G.neighborhood(root);

    if (par != -1)
        nb.erase(std::find(nb.begin(), nb.end(), par));
    if (nb.empty())
        return;

    this->tree_contractor(nb[0], root);
    for (int i = 1; i < (int) nb.size(); i++) {
        this->tree_contractor(nb[i], root);
        this->cs.emplace_back(nb[0], nb[i]);
    }

    this->cs.emplace_back(root, nb[0]);
}

void Solver::solve() {
    this->solve_sat();
}

void Solver::print_contraction() {
    for (auto [u, p] : this->cs)
        std::cout << u << " " << p << std::endl;
}

ContractionSequence Solver::get_contraction() {
    return this->cs;
}

ContractionSequence Solver::solve_sat(int lb, int ub) {
    int n = G.count_vertices();
    if (ub == -1) ub = n - 1;

    SatSolver solver;

    // TODO: There will be a lot of repeated code
    // Maybe it is worth to refactor it
    // Indexing o-variables
    std::map<std::pair<int, int>, int> o_variables;
    auto o = [&] (int i, int j) {
        assert(i != j);
        bool sign = false;
        if (i > j) {
            std::swap(i, j);
            sign = true;
        }
        assert(o_variables.count(std::pair(i, j)));
        return (sign ? -1 : 1) * o_variables[std::pair(i, j)];
    };
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            o_variables[std::pair(i, j)] = solver.new_variable();
        }
    }

    // Indexing p-variables
    std::map<std::pair<int, int>, int> p_variables;
    auto p = [&] (int i, int j) {
        assert(i != j);
        bool sign = false;
        if (i > j) {
            std::swap(i, j);
            sign = true;
        }
        assert(p_variables.count(std::pair(i, j)));
        return (sign ? -1 : 1) * p_variables[std::pair(i, j)];
    };
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            p_variables[std::pair(i, j)] = solver.new_variable();
        }
    }

    // Indexing a-variables
    std::map<std::pair<int, int>, int> a_variables;
    auto a = [&] (int i, int j) {
        assert(i != j);
        bool sign = false;
        if (i > j) {
            std::swap(i, j);
            sign = true;
        }
        assert(a_variables.count(std::pair(i, j)));
        return (sign ? -1 : 1) * a_variables[std::pair(i, j)];
    };
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            a_variables[std::pair(i, j)] = solver.new_variable();
        }
    }

    // Indexing r-variables
    std::map<std::tuple<int, int, int>, int> r_variables;

    auto r = [&] (int i, int j, int k) {
        assert(j != k);
        bool sign = false;
        if (j > k) {
            std::swap(j, k);
            sign = true;
        }
        assert(r_variables.count(std::tuple(i, j, k)));
        return (sign ? -1 : 1) * r_variables[std::tuple(i, j, k)];
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                r_variables[std::tuple(i, j, k)] = solver.new_variable();
            }
        }
    }

    // Creating model restrictions 

    // restriction: transitivity of the order
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (i != j && i != k && j != k) {
                    std::vector<int> clause = {
                        -o(i, j), -o(j, k), o(i, k)
                    };
                    solver.add_clause(clause);
                }
            }
        }
    }

    // restriction: at least one parent per vertex
    for (int i = 0; i < n - 1; i++) {
        std::vector<int> clause = {
        };
        for (int j = i + 1; j < n; j++) {
            clause.push_back(p(i, j));
        }
        solver.add_clause(clause);
    }

    // restriction: at most one parent per vertex
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (i != j && i != k && j != k) {
                    std::vector<int> clause = {
                        -p(i, j), -p(i, k)
                    };
                    solver.add_clause(clause);
                }
            }
        }
    }

    // restriction: the parent comes next in the order
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                std::vector<int> clause = {
                    -p(i, j), o(i, j)
                };
                solver.add_clause(clause);
            }
        }
    }

     restriction: bookkeeping of 'r' and 'a'
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (i != k && j != k) {
                    std::vector<int> clause = {
                        -o(i, j), -o(i, k), -r(i, j, k), a(j, k)
                    };
                    solver.add_clause(clause);
                }
            }
        }
    }

    // restriction: edges in the symmetric difference
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k : G.symmetric_difference(i, j)) {
                std::vector<int> clause = {
                    -p(i, j), -o(i, k), r(i, j, k)
                };
                solver.add_clause(clause);
            }
        }
    }

    // restriction: inheritance of edges
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (i != k && j != k) {
                    std::vector<int> clause = {
                        -p(i, j), -o(i, k), -a(i, k), r(i, j, k)
                    };
                    solver.add_clause(clause);
                }
            }
        }
    }

    // restriction: edges from H_i keep existing in H_{i+1}
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                for (int m = k + 1; m < n; m++) {
                    if (i != j && i != k && i != m && j != k && j != m) {
                        std::vector<int> clause = {
                            -o(i, j), -o(j, k), -o(j, m), -r(i, k, m), r(j, k, m)
                        };
                        solver.add_clause(clause);
                    }
                }
            }
        }
    }

    std::cout << "Solve code: " << solver.solve() << std::endl;

    return {};
}
