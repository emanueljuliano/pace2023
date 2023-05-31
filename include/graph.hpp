#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <iostream>

#include <cassert>
#include <numeric>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>

typedef std::pair<int, int> Edge;
typedef std::vector<std::pair<int, int>> ContractionSequence;

class Graph {
   public:
    Graph(int n);
    Graph(const Graph& H);
    void add_edge(int u, int v);
    void remove_edge(int u, int v);
	bool has_edge(int u, int v) const;
    int count_edges() const;
    int count_vertices() const;
    std::vector<int> neighborhood(int u) const;
    std::vector<int> symmetric_difference(int u, int v) const;
    std::vector<std::vector<int>> adjacency_list() const;
    std::vector<Edge> edges() const;
    int degree(int u) const;
    std::vector<int> degree() const;
    int label(int u) const;
    bool is_connected() const;
    bool is_tree() const;
    bool is_bipartite() const;
    bool is_planar() const; // TODO
    Graph complement() const;
    std::pair<std::vector<Graph>, std::vector<int>> decompose() const; // TODO
    void decompose(std::vector<Graph>& g, std::vector<int>& co_tree, int parent) const; // TODO
    ContractionSequence recompose(std::vector<std::pair<ContractionSequence, int>>& seq, std::vector<int>& co_tree) const;
    int width(const ContractionSequence& seq) const;
	int greedy_upper_bound() const;
	int greedy_lower_bound() const;

   private:
	std::pair<std::vector<Edge>, std::vector<Edge>> contraction_edges(const std::vector<bool>&, const Graph& H, int p, int u) const;

    std::vector<std::vector<bool>> g;
    std::vector<int> deg;
    std::vector<int> labels;
    int m;
};

#endif // ! __GRAPH_HPP
