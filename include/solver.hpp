#ifndef __SOLVER_HPP
#define __SOLVER_HPP

#include "graph.hpp"

// Finds the TWW contraction sequence for a connected graph
class Solver {
   public:
    Solver(Graph& g);
    void solve();

   private:
    int tree_verifier();
    Graph& g;
};

#endif // ! __SOLVER_HPP
