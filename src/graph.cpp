#include "../include/graph.hpp"
#include <iostream>

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

std::vector<Graph> Graph::subgraphs(std::vector<std::vector<int>> comp) const {
	const Graph& G = *this;
	int n = G.count_vertices();
	std::vector<int> compressed(n);
	std::vector<Graph> ret;
	std::vector<int> id(n);

	for (int i = 0; i < int(comp.size()); i++) {
		std::vector<int> cmp = comp[i];
		for (int j = 0; j < int(cmp.size()); j++) {
			compressed[cmp[j]] = j;
			id[cmp[j]] = i;
		}
	}
	
	for (std::vector<int> cmp : comp) {
		Graph h(cmp.size());
		for (int u : cmp) {
			h.labels[compressed[u]] = G.labels[u];
			for (int v : G.neighborhood(u)) if (id[v] == id[u]) {
				h.add_edge(compressed[u], compressed[v]);
			}
		}
		ret.push_back(h);
	}
	return ret;
}

std::vector<std::vector<int>> Graph::refine(std::vector<std::vector<int>> &partition, std::vector<int> pivot) const {
	std::sort(pivot.begin(), pivot.end());
	std::vector<std::vector<int>> refinement;
	for (std::vector<int> &X : partition) {
		std::vector<int> in, out;
		for (int x : X) {
			if (std::binary_search(pivot.begin(), pivot.end(), x)) {
				in.push_back(x);
			} 
			else out.push_back(x);
		}	

		if (in.size()) refinement.push_back(in);
		if (out.size()) refinement.push_back(out);
	}
	return refinement;
}

bool perp(std::vector<int> &A, std::vector<int> &B) {
	int inter = 0;
	bool diffA = false, diffB = false;
	for (int b : B) {
		if (std::binary_search(A.begin(), A.end(), b)) inter++;
		else diffB = true;
	}
	
	if (inter < A.size()) diffA = true;

	return diffA and diffB and inter;
}

void print_partition(std::vector<std::vector<int>>& partition) {
	for (auto P : partition) {
		for (int p : P) std::cout << p << " ";
		std::cout << std::endl;
	}
}

std::vector<std::vector<int>> Graph::modular_partition(std::vector<std::vector<int>> &P) const {
    const Graph& G = *this;
	std::vector<std::vector<int>> Q = P, K, L;
	for (std::vector<int> Z : P) {
		if (K.empty()) {
			K.push_back(Z);
			continue;
		}
		if (K[0].size() < Z.size()) swap(K[0], Z);
		L.push_back(Z);
	}
	
	int iteration = 0;
	while (L.size() or K.size()) {
		/*std::cout << "\nIteration: " << iteration++ << std::endl;
		std::cout << "Q" << std::endl;
		print_partition(Q);
		std::cout << "K" << std::endl;
		print_partition(K);
		std::cout << "L" << std::endl;
		print_partition(L);
		*/
		std::vector<int> S;
		if (L.size()) {
			S = L.back(); L.pop_back();
		}
		else {
			S.push_back(K[0][0]);
			K.erase(K.begin());
		}

		for (int x : S) {
			std::vector<int> neig = G.neighborhood(x);
			std::sort(neig.begin(), neig.end());
			int q_size = Q.size();
			for (int y_id = 0; y_id < q_size; y_id++) {
				std::vector<int> &Y = Q[y_id];
				bool is_x = false;
				for (int y : Y) if (y == x) is_x = true;

				if (is_x) {
					continue;
				}

				std::vector<int> in, out;
				for (int y : Y) {
					if (std::binary_search(neig.begin(), neig.end(), y))
						in.push_back(y);
					else out.push_back(y);
				}
				
				if (in.empty() or out.empty()) continue;

				std::vector<int> Y_min = in, Y_max = out;
				if (Y_min.size() > Y_max.size()) std::swap(Y_min, Y_max);
				
				int idx_l = -1, idx_k = -1;
				for (int i = 0; i < int(L.size()); i++) if (Y[0] == L[i][0]) idx_l = i;
				for (int i = 0; i < int(K.size()); i++) if (Y[0] == K[i][0]) idx_k = i;

				if (idx_l != -1) {
					L[idx_l] = Y_min;
					L.push_back(Y_max);
				}
				else {
					L.push_back(Y_min);
					if (idx_k != -1) {
						K[idx_k] = Y_max;
					}
					else K.push_back(Y_max);
				}
				
				Y = Y_max;
				Q.push_back(Y_min);
			}
		}
	}
	// std::cout << "Final partition: " << std::endl;
	// print_partition(Q);
	return Q;
}

std::vector<std::vector<int>> Graph::prime_decomposition() const {
	int n = this->g.size();
	int x = rand() % n;
	std::vector<std::vector<int>> partition;
	partition.push_back({x});
	std::vector<int> in, out;
	for (int y = 0; y < n; y++) if (y != x) {
		if (this->g[x][y]) in.push_back(y);
		else out.push_back(y);
	}
	partition.push_back(in);
	partition.push_back(out);
	std::vector<std::vector<int>> modular =  modular_partition(partition);

	for (std::vector<int> mod : modular) {
		std::vector<int> cnt(n);
		for (int i : mod) cnt[i] = -1;
		for (int i : mod) for (int j : this->neighborhood(i)) if (cnt[j] != -1)
			cnt[j]++;
		
		for (int i = 0; i < n; i++) assert(cnt[i] <= 0 or cnt[i] == mod.size());
	}
	return modular;
}

void Graph::decompose(
    std::vector<Graph>& decomposition, std::vector<std::pair<int, std::vector<int>>>& modular_tree, int parent
) const {
    const Graph& G = *this;
    int n = G.count_vertices();

    int tree_index = modular_tree.size();
	modular_tree.emplace_back();
	modular_tree[tree_index].first = -1;
    if (parent != -1) modular_tree[parent].second.push_back(tree_index);

	if (n == 1) {
		decomposition.push_back(G);
	}
    else if (!G.is_connected()) {
        std::vector<int> vis(n, 0);
        std::vector<int> stack;
        std::vector<std::vector<int>> comp;

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
        }
        for (Graph h : G.subgraphs(comp)) {
			h.decompose(decomposition, modular_tree, tree_index);
    	}
	}
    else if (!G.complement().is_connected()) {
        G.complement().decompose(decomposition, modular_tree, tree_index);
    }
    else {
        std::vector<std::vector<int>> partition = G.prime_decomposition();
		
		if (int(partition.size()) == n) {
			decomposition.push_back(G);
    		return;
		}
		
        for (Graph h : G.subgraphs(partition)) {
			h.decompose(decomposition, modular_tree, tree_index);
    	}
		
		// std::cout << "Current partition " << std::endl;
		// print_partition(partition);
		Graph Q(partition.size());
		for (int i = 0; i < int(partition.size()); i++) {
			int a = partition[i][0];
			for (int j = i+1; j < int(partition.size()); j++) {
				int b = partition[j][0];
				if (G.g[a][b]) {
					Q.add_edge(i, j);
					// std::cout << "edge " << i << " " << j << " " << std::endl; 
				}
			}
		}
		assert(Q.is_connected());
		modular_tree[tree_index].first = decomposition.size();
		decomposition.push_back(Q);
	}
}

std::pair<std::vector<Graph>,  std::vector<std::pair<int, std::vector<int>>>> Graph::decompose() const {
    std::vector<Graph> decomposition;
    std::vector<std::pair<int, std::vector<int>>> modular_tree;
    this->decompose(decomposition, modular_tree, -1);
    return std::pair(decomposition, modular_tree);
}

ContractionSequence Graph::recompose(
    std::vector<std::pair<ContractionSequence, int>>& seq, std::vector<std::pair<int, std::vector<int>>>& modular_tree
) const {
    ContractionSequence ret;
    int n = modular_tree.size();
	std::vector<int> rep(n);
	int pos = 0;
	std::function<void(int)> dfs = [&](int v) {
		auto [type, neig] = modular_tree[v];

		if (neig.empty()) { // folha
			for (std::pair<int, int> p : seq[pos].first) {
				ret.push_back(p);
			}
			rep[v] = seq[pos].second;
			pos++;
			return;
		}
		
		std::vector<int> children;
		for (int u : neig) {
			dfs(u);
			children.push_back(rep[u]);
		}
		
		if (modular_tree[v].first == -1) {
			for (int i = 1; i < int(children.size()); i++) {
				ret.emplace_back(children[0], children[i]);
			}
			rep[v] = children[0];
		}
		else {
			for (auto [a, b] : seq[type].first) {
				ret.emplace_back(children[a], children[b]);
			}
			rep[v] = children[seq[type].second];
		}
	};

	dfs(0);
	
    return ret;
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
