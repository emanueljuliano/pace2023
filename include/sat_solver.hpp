#ifndef __SAT_SOLVER_HPP
#define __SAT_SOLVER_HPP

#include <vector>

#include "cadical.hpp"

class SatSolver {
   public:
    SatSolver();
    SatSolver(const SatSolver& s);
    int new_variable();
    void add_clause(std::vector<int> clause);
    void add_auxiliary_constraint(std::vector<int> variables); // will call only once per object
    void add_cardinality_constraint(int d);
    
   private:
    int n;
    CaDiCaL::Solver solver;
    std::vector<int> constraints;
};

#endif // ! __SAT_SOLVER_HPP
