#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <vector>
#include <utility>
#include <cassert>

typedef std::pair<int, int> Edge;
typedef std::vector<std::pair<int, int>> ContractionSequence;

class Graph {
   public:
    Graph(int n);
    Graph(const Graph& H);
    void add_edge(int u, int v);
    void remove_edge(int u, int v);
    int count_edges() const;
    int count_vertices() const;
    std::vector<int> neighborhood(int u) const;
    std::vector<int> symmetric_difference(int u, int v) const;
    std::vector<std::vector<int>> adjacency_list() const;
    std::vector<Edge> edges() const;
    int degree(int u) const;
    std::vector<int> degree() const;
    bool is_connected() const;
    int width(const ContractionSequence& seq) const;

   private:
    std::vector<std::vector<bool>> g;
    std::vector<int> deg;
    int m;
};

#endif // ! __GRAPH_HPP
