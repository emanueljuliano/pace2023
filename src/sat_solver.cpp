#include "../include/sat_solver.hpp"

#include <cassert>
#include <functional>
#include <cmath>
#include <vector>

SatSolver::SatSolver() : index(0), solver(), constraints() {
    this->t = this->new_variable();
    this->f = this->new_variable();
    // ASSUME BEFORE SOLVING?!
}

int SatSolver::new_variable() {
	return ++(this->index);
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

void SatSolver::add_auxiliary_constraints(std::vector<int> variables) {
    int n = variables.size();
    std::vector<std::vector<int>> tree(4 * n);
    std::vector<int> leaves;

    for (auto& node : tree)
        node.push_back(this->t);

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
    for (int i = 1; i <= n; i++)
        this->constraints.push_back(tree[1][i]);

    for (auto& node : tree)
        node.push_back(this->f);

    for (int i = 0; i < 4 * n; i++) if (tree[i].size() > 3) {
        int l = 2 * i, r = 2 * i + 1;
        for (int a = 0; a <= (int) tree[l].size() - 2; a++) {
            for (int b = 0; b <= (int) tree[r].size() - 2; b++) {
                this->add_clause({
                    -tree[l][a],
                    -tree[r][b],
                    tree[i][a + b]}
                );
                this->add_clause({
                    tree[l][a + 1],
                    tree[r][b + 1],
                    -tree[i][a + b + 1]}
                );
            }
        }
    }
}

void SatSolver::add_cardinality_constraints(int ub, int lb) {
    assert(!this->constraints.empty());
    // lower bound constraints
    for (int i = 0; i < lb; i++)
        this->add_clause({this->constraints[i]});
    // upper bound constraints
    for (int i = (int)this->constraints.size() - 1; i >= ub; i--)
        this->add_clause({-this->constraints[i]});
}
