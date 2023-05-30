#include <iostream>
#include <fstream>
#include <string>

#include "../include/graph.hpp"
#include "../include/solver.hpp"
#include "../include/sat_solver.hpp"

// TODO: IO Class?
Graph* read_graph(std::string& filename) {
    std::ifstream in(filename);

    std::string s;  
    in >> s >> s;
    
    int n, m;
    in >> n >> m;

    Graph* g = new Graph(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        in >> a >> b;
        g->add_edge(a - 1, b - 1);
    }

    return g;
}

ContractionSequence read_contraction_sequence(std::string& filename, int length) {
    ContractionSequence sequence(length);

    std::ifstream in(filename);
    for (int i = 0; i < length; i++) {
        in >> sequence[i].first >> sequence[i].second;  
        sequence[i].first--;
        sequence[i].second--;
    }

    return sequence;
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
    if (argc <= 1)
        return 1;

    std::string input_file = argv[1];

    Graph* g = read_graph(input_file);
   	
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
