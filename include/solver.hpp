#ifndef __SOLVER_HPP
#define __SOLVER_HPP

#include "graph.hpp"

// Finds the TWW contraction sequence for a connected graph
class Solver {
   public:
    Solver(Graph& G);
    void solve();

   private:
    int tree_verifier();
    ContractionSequence tree_contractor(int root);
    Graph& G;
};

#endif // ! __SOLVER_HPP
