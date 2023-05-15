#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <vector>

class Graph {
   public:
    Graph(int n);
    void add_edge(int u, int v);

   private:
    std::vector<std::vector<int>> g;
};

#endif // ! __GRAPH_HPP
