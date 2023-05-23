#include "../include/sat_solver.hpp"

#include <cassert>
#include <functional>
#include <cmath>
#include <vector>

#include <iostream>

SatSolver::SatSolver() : n(0), solver(), constraints() {}

int SatSolver::new_variable() {
	return ++(this->n);
}

void SatSolver::add_clause(std::vector<int> clause) {
	for (auto var: clause) {
		(this->solver).add(var);
	}
	(this->solver).add(0);
}

int SatSolver::solve() {
    return solver.solve();
}

void SatSolver::add_auxiliary_constraint(std::vector<int> variables) {
    int n = variables.size();
    std::vector<std::vector<int>> tree(4 * n);
    std::vector<int> leaves;

    std::function<void(int, int)> build = [&] (int node, int size) {
        if (size == 1)
            leaves.push_back(node);
        else {
            for (int i = 0; i < size; i++)
                tree[node].push_back(this->new_variable());
            build(2 * node, std::floor((float) size / 2));
            build(2 * node + 1, std::ceil((float) size / 2));
        }
    };
    build(1, n);

    for (int i = 0; i < n; i++)
        tree[leaves[i]].push_back(variables[i]);
    assert(this->constraints.empty());
    for (int i = 0; i < n; i++)
        this->constraints.push_back(tree[1][i]);
}

void SatSolver::add_cardinality_constraint(int ub, int lb) {
    assert(!this->constraints.empty());
    // lower bound constraints
    for (int i = 0; i < lb; i++)
        this->add_clause({this->constraints[i]});
    // upper bound constraints
    for (int i = (int)this->constraints.size() - 1; i >= ub; i--)
        this->add_clause({-this->constraints[i]});
}
