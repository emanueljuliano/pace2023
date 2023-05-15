#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <vector>

class Graph {
   public:
    Graph(int n);
    void add_edge(int u, int v);
    int count_edges();
    int count_vertices();

   private:
    std::vector<std::vector<int>> g;
    int m;
};

#endif // ! __GRAPH_HPP
