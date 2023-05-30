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

ContractionSequence contract_and_recompose(Graph* g) {
	auto [decomposition, co_tree] = g->decompose();
	std::vector<std::pair<ContractionSequence, int>> seq;

	for (Graph h : decomposition) {
		Solver s(h);
		s.solve();
		ContractionSequence cs = s.get_contraction();
		int rep;
		if (cs.size() == 0) rep = h.label(0);
		else rep = cs.back().first;
		seq.emplace_back(cs, rep);
	}

	return g->recompose(seq, co_tree);
}

int main(int argc, char* argv[]) {
    Graph* g = read_graph();
   	
    // Solver s(*g);
    // s.solve();
	
	ContractionSequence result = contract_and_recompose(g);
    auto w = g->width(result);
    
	std::cerr << "Twin Width: " << w << std::endl;

	// s.print_contraction();
	for (auto [a, b] : result) {
		std::cout << a+1 << " " << b+1 << std::endl;
	}

    delete g;
    return 0;
}
