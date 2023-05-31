#include "../include/graph.hpp"

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

bool Graph::has_edge(int u, int v) const {
    assert(u != v);
	return this->g[u][v];
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
        if (i != u && i != v && g[u][i] != g[v][i]) {
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

// Returns the set of edges to be removed, and the set of edges to be added in a contraction
std::pair<std::vector<Edge>, std::vector<Edge>>
Graph::contraction_edges(
		const std::vector<bool>& removed, const Graph& H, int p, int u
) const {
	std::vector<Edge> to_remove, to_add;
	for (int x: H.neighborhood(u)) {
		to_remove.emplace_back(u, x);
		if (x != p && !H.has_edge(p, x))
			to_add.emplace_back(p, x);
	}
	for (int x: this->symmetric_difference(u, p)) {
		if (!removed[x] and !H.has_edge(p, x))
			to_add.emplace_back(p, x);
	}

	return std::pair(to_remove, to_add);
}

int Graph::greedy_width() const {
	const int n = this->count_vertices();
	Graph H(n);
	int answer = 0;
	std::vector<bool> removed(n, false);

	ContractionSequence seq;
	for (int iter = 0; iter < n - 1; iter++) {
		std::tuple<int, int, int> best_choice(n, 0, 0);
		for (int p = 0; p < n; p++) if (!removed[p]) {
			for (int u = p + 1; u < n; u++) if (!removed[u]) {
				auto [to_remove, to_add] = this->contraction_edges(removed, H, p, u);

				for (auto [x, y] : to_remove)
					H.remove_edge(x, y);

				for (auto [x, y] : to_add)
					H.add_edge(x, y);

				int max_deg = 0;
				for (int i = 0; i < n; i++)
					max_deg = std::max(max_deg, H.degree(i));
				best_choice = std::min(best_choice, std::tuple(max_deg, p, u));

				for (auto [x, y] : to_remove)
					H.add_edge(x, y);

				for (auto [x, y] : to_add)
					H.remove_edge(x, y);
			}
		}

		auto [max_deg, p, u] = best_choice;
		assert(max_deg != n && p != u);
		seq.emplace_back(p, u);

		answer = std::max(answer, max_deg);

		auto [to_remove, to_add] = this->contraction_edges(removed, H, p, u);
		removed[u] = true;
		for (auto [x, y] : to_remove)
			H.remove_edge(x, y);

		for (auto [x, y] : to_add)
			H.add_edge(x, y);
	}
	assert(this->width(seq) == answer);
	std::cerr << "Greedy Upper Bound: " << answer << std::endl;

	return answer;
}

int Graph::width(const ContractionSequence& seq) const {
	Graph H(this->count_vertices());
	int answer = 0;
	std::vector<bool> removed(this->count_vertices(), false);
	for (auto [p, u] : seq) {
		auto [to_remove, to_add] = this->contraction_edges(removed, H, p, u);
		removed[u] = true;
		for (auto [x, y] : to_remove)
			H.remove_edge(x, y);

		for (auto [x, y] : to_add)
			H.add_edge(x, y);

		for (int i = 0; i < this->count_vertices(); i++)
			answer = std::max(answer, H.degree(i));
	}
	return answer;
}
