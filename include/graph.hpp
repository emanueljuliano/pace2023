#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <vector>
#include <utility>
#include <cassert>

typedef std::pair<int, int> Edge;
typedef std::vector<Edge> ContractionSequence;

class Graph {
   public:
    Graph(int n);
    void add_edge(int u, int v);
    void remove_edge(int u, int v);
    int count_edges();
    int count_vertices();
    std::vector<int> neighborhood(int u);
    std::vector<int> symmetric_difference(int u, int v);
	std::vector<Edge> edges();
	int degree(int u);
	int width(ContractionSequence& seq);

   private:
    std::vector<std::vector<bool>> g;
	std::vector<int> deg;
    int m;
};

#endif // ! __GRAPH_HPP
