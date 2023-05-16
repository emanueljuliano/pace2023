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
    int count_edges() const;
    int count_vertices() const;
    std::vector<int> neighborhood(int u) const;
    std::vector<int> symmetric_difference(int u, int v) const;
    std::vector<Edge> edges() const;
    int degree(int u) const;
    bool is_connected() const;
    int width(const ContractionSequence& seq) const;

   private:
    std::vector<std::vector<bool>> g;
    std::vector<int> deg;
    int m;
};

#endif // ! __GRAPH_HPP
