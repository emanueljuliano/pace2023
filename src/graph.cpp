#include "../include/graph.hpp"

#include <vector>

Graph::Graph(int n) {
    this->g.assign(n, std::vector<bool>(n, false));
    this->deg.assign(n, 0);
    this->labels.assign(n, 0);
    std::iota(this->labels.begin(), this->labels.end(), 0);
    this->m = 0;
}

Graph::Graph(const Graph &H) {
    int n = H.count_vertices();
    this->g.assign(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (H.g[i][j]) this->g[i][j] = true;
        }
    }
    this->deg.assign(n, 0);
    this->labels.assign(n, 0);
    for (int i = 0; i < n; i++) {
        this->deg[i] = H.deg[i];
        this->labels[i] = H.labels[i];
    }
    this->m = H.m;
}

void Graph::add_edge(int u, int v) {
    assert(u != v);
    if (this->g[u][v]) return;
    this->g[u][v] = this->g[v][u] = true;
    this->deg[u]++, this->deg[v]++;
    this->m++;
}

void Graph::remove_edge(int u, int v) {
    assert(u != v);
    if (!this->g[u][v]) return;
    this->g[u][v] = this->g[v][u] = false;
    this->deg[u]--, this->deg[v]--;
    this->m--;
}

int Graph::count_edges() const {
    return this->m;
}

int Graph::count_vertices() const {
    return this->g.size();
}

std::vector<int> Graph::neighborhood(int u) const {
    std::vector<int> answer;
    for (int i = 0; i < (int)g.size(); i++) {
        if (g[u][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<int> Graph::symmetric_difference(int u, int v) const {
    std::vector<int> answer;
    for (int i = 0; i < (int)g.size(); i++) {
        if (i != u && i != v && g[u][i] ^ g[v][i]) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<Edge> Graph::edges() const {
    std::vector<Edge> answer;
    for (int i = 0; i < this->count_vertices(); i++) {
        for (int j = i + 1; j < this->count_vertices(); j++) {
            if (this->g[i][j])
                answer.emplace_back(i, j);
        }
    }

    return answer;
}

int Graph::degree(int v) const {
    return this->deg[v];
}

std::vector<int> Graph::degree() const {
    return this->deg;
}

int Graph::label(int v) const {
    return this->labels[v];
}

bool Graph::is_connected() const {
    const Graph& g = *this;

    int n = g.count_vertices();
    std::vector<int> vis(n, 0);
    std::vector<int> stack;
    stack.push_back(0);

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (!vis[u]) {
            vis[u] = 1;
            for (auto v : g.neighborhood(u)) if (!vis[v])
                stack.push_back(v);
        }
    }

    for (auto u : vis) if (!u) 
        return false;
    return true;
}

bool Graph::is_bipartite() const {
    const Graph& g = *this;
    int n = g.count_vertices();
    std::vector<int> color(n, -1);
    std::vector<int> stack;
    stack.push_back(0);
	color[0] = 0;

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int v : g.neighborhood(u)) {
            if (color[v] == -1) {
                color[v] = !color[u];
                stack.push_back(v);
            }
            else if (color[v] == color[u]) {
                return false;
            }
        }
    }
    return true;
}

Graph Graph::complement() const {
    const Graph& g = *this;
    int n = g.count_vertices();
	Graph h(g);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) if (i != j) {
			h.g[i][j] = !h.g[i][j];
        }
	}
	return h;
}

void Graph::decompose(
	std::vector<Graph>& decomposition, std::vector<int>& co_tree, int parent
) const {
    const Graph& g = *this;
    int n = g.count_vertices();
	
	int tree_index = co_tree.size();
	co_tree.push_back(parent);
	
	if (!g.is_connected()) {
    	std::vector<int> vis(n, 0);
   	 	std::vector<int> stack;
		std::vector<std::vector<int>> comp;
		std::vector<int> compressed(n);

		for (int i = 0; i < n; i++) if (!vis[i]) {
			stack.push_back(i);
			vis[i] = 1;
			comp.emplace_back(1, i);

			while (stack.size()) {
				int u = stack.back();
				stack.pop_back();
				for (int v : g.neighborhood(u)) if (!vis[v]) {
					vis[v] = 1;
					stack.push_back(v);
					comp.back().push_back(v);
				}
			}
			
			for (int j = 0; j < int(comp.back().size()); j++) {
				compressed[comp.back()[j]] = j;
			}
		}
		
		for (std::vector<int> cmp : comp) {
			Graph h(cmp.size());
			for (int u : cmp) {
				h.labels[compressed[u]] = g.labels[u];
				for (int v : g.neighborhood(u)) {
					h.add_edge(compressed[u], compressed[v]);
				}
			}
			h.decompose(decomposition, co_tree, tree_index);
		}
	}
	else if (!g.complement().is_connected()) {
		g.complement().decompose(decomposition, co_tree, tree_index);
	}
	else {
		decomposition.push_back(g);
	}
}

std::pair<std::vector<Graph>, std::vector<int>> Graph::decompose() const {
	std::vector<Graph> decomposition;
	std::vector<int> co_tree;
	this->decompose(decomposition, co_tree, -1);
	return std::pair(decomposition, co_tree);
}

bool Graph::is_tree() const {
    const int n = this->count_vertices();
    const int m = this->count_edges();
    return this->is_connected() && m == n - 1;
}

std::vector<std::vector<int>> Graph::adjacency_list() const {
    int n = this->count_vertices();
    std::vector<std::vector<int>> adj(n);
    for (int u = 0; u < n; u++) {
        for (auto v : this->neighborhood(u))
            adj[u].push_back(v);
    }
    return adj;
}

int Graph::width(const ContractionSequence& seq) const {
    Graph H(this->count_vertices());
    int answer = 0;
    std::vector<bool> erased(this->count_vertices(), false);
    for (auto [p, u] : seq) {
        erased[u] = true;
        for (int x: H.neighborhood(u)) {
            H.remove_edge(u, x);
            if (x != p)
                H.add_edge(p, x);
        }
        for (int x: this->symmetric_difference(u, p)) {
            if (!erased[x])
                H.add_edge(p, x);
        }
        for (int i = 0; i < this->count_vertices(); i++)
            answer = std::max(answer, H.degree(i));
    }
    return answer;
}
