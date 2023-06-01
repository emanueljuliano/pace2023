#include <iostream>
#include <fstream>
#include <string>

#include "../include/graph.hpp"
#include "../include/solver.hpp"
#include "../include/sat_solver.hpp"

// TODO: IO Class?
Graph* read_graph() {
    std::string s;  
	std::cin >> s >> s;
    
    int n, m;
	std::cin >> n >> m;

    Graph* g = new Graph(n);

    for (int i = 0; i < m; i++) {
        int a, b;
		std::cin >> a >> b;
        g->add_edge(a - 1, b - 1);
    }

    return g;
}

ContractionSequence contract_and_recompose(Graph* g, int& max_width) {
	auto [decomposition, modular_tree] = g->decompose();
	std::vector<std::pair<ContractionSequence, int>> seq;
	// std::cerr << "Decomposition size: " << decomposition.size() << std::endl;
	for (Graph h : decomposition) {
		// std::cerr << "Size of h: " << h.count_vertices() << std::endl;
		Solver s(h);
		ContractionSequence cs;
		s.solve();
		cs = s.get_contraction();
		max_width = std::max(max_width, h.width(cs));

		int rep;
		for (auto &[a, b] : cs) {
			// std::cerr << a << " " << b << std::endl;
			a = h.label(a), b = h.label(b);
		}
		if (cs.size() == 0) rep = h.label(0);
		else rep = cs.back().first;
		seq.emplace_back(cs, rep);
	}
	return g->recompose(seq, modular_tree);
}

int main(int argc, char* argv[]) {
    Graph* g = read_graph();
   	
	int max_width = 0;
	ContractionSequence result = contract_and_recompose(g, max_width);
    auto w = g->width(result);
	assert(w == max_width);
    
	// std::cerr << "Twin Width: " << w << std::endl;

	for (auto [a, b] : result) {
		std::cout << a+1 << " " << b+1 << std::endl;
	}

    delete g;
    return 0;
}
