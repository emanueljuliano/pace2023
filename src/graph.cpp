#include "../include/graph.hpp"

#include <vector>
#include <queue>

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
    const Graph& G = *this;

    int n = G.count_vertices();
    std::vector<int> vis(n, 0);
    std::vector<int> stack;
    stack.push_back(0);

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (!vis[u]) {
            vis[u] = 1;
            for (auto v : G.neighborhood(u)) if (!vis[v])
                stack.push_back(v);
        }
    }

    for (auto u : vis) if (!u)
        return false;
    return true;
}

bool Graph::is_bipartite() const {
    const Graph& G = *this;
    int n = G.count_vertices();
    std::vector<int> color(n, -1);
    std::vector<int> stack;
    stack.push_back(0);
	color[0] = 0;

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int v : G.neighborhood(u)) {
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
    const Graph& G = *this;
    int n = G.count_vertices();
	Graph H(G);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) if (i != j) {
			H.g[i][j] = !H.g[i][j];
        }
	}
	return H;
}

void Graph::decompose(
	std::vector<Graph>& decomposition, std::vector<int>& co_tree, int parent
) const {
    const Graph& G = *this;
    int n = G.count_vertices();

	int tree_index = co_tree.size();
	co_tree.push_back(parent);

	if (!G.is_connected()) {
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
				for (int v : G.neighborhood(u)) if (!vis[v]) {
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
				h.labels[compressed[u]] = G.labels[u];
				for (int v : G.neighborhood(u)) {
					h.add_edge(compressed[u], compressed[v]);
				}
			}
			h.decompose(decomposition, co_tree, tree_index);
		}
	}
	else if (!G.complement().is_connected()) {
		G.complement().decompose(decomposition, co_tree, tree_index);
	}
	else {
		decomposition.push_back(G);
	}
}

ContractionSequence Graph::recompose(
	std::vector<std::pair<ContractionSequence, int>>& seq, std::vector<int>& co_tree
) const {
	ContractionSequence ret;

	int n = co_tree.size();
	std::vector<int> dg(n);

	for (int i = 1; i < n; i++) dg[co_tree[i]]++;

	std::vector<std::vector<int>> to_contract(n);
	std::queue<int> to_process;
	for (int i = 0, pos = 0; i < n; i++) if (dg[i] == 0) {
		for (std::pair<int, int> p : seq[pos].first) {
			ret.push_back(p);
		}
		int rep = seq[pos].second;
		pos++;
		to_process.push(i);
		to_contract[i].push_back(rep);
	}

	// BFS on co_tree starting from leaves
	while (to_process.size()) {
		int u = to_process.front(); to_process.pop();

		while (to_contract[u].size() > 1) {
			int a = to_contract[u].back(); to_contract[u].pop_back();
			int b = to_contract[u].back();
			ret.emplace_back(b, a);
		}
		
		if (u == 0) {
			break;
		}
		
		int v = co_tree[u];
		to_contract[v].push_back(to_contract[u].back());
		dg[v]--;

		if (dg[v] == 0) to_process.push(v);
	}

	return ret;
}


std::pair<std::vector<Graph>, std::vector<int>> Graph::decompose() const {
	std::vector<Graph> decomposition;
	std::vector<int> co_tree;
	this->decompose(decomposition, co_tree, -1);
	return std::pair(decomposition, co_tree);
}

bool Graph::is_tree() const {
    const int _n = this->count_vertices();
    const int _m = this->count_edges();
    return this->is_connected() && _m == _n - 1;
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
