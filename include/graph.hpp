#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <vector>
#include <utility>

typedef std::vector<std::pair<int, int>> ContractionSequence;

class Graph {
   public:
    Graph(int n);
    void add_edge(int u, int v);
    int count_edges();
    int count_vertices();
    std::vector<int> neighborhood(int u);
    std::vector<int> symmetric_difference(int u, int v);
	int width(ContractionSequence& seq);

   private:
    std::vector<std::vector<int>> g;
    int m;
};

#endif // ! __GRAPH_HPP
