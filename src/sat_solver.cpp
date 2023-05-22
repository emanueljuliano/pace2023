#include "../include/sat_solver.hpp"

SatSolver::SatSolver() : n(0), solver(), constraints() {}

int SatSolver::new_variable() {
	return (this->n)++;
}

void SatSolver::add_clause(std::vector<int> clause) {
	for (auto var: clause) {
		(this->solver).add(var);
	}
	(this->solver).add(0);
}
