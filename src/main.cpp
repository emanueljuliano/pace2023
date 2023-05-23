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

int main(int argc, char* argv[]) {
    if (argc <= 1)
        return 1;

    std::string input_file = argv[1];

    Graph* g = read_graph(input_file);
   	
	auto [decomposition, co_tree] = g->decompose();
	
	std::cout << "Decomposition: " << std::endl;
	for (Graph h : decomposition) {
		std::cout << "\nNew component\n";
		for (int i = 0; i < h.count_vertices(); i++) {
			std::cout << "vertex: " << i << " label: " << h.label(i);
			std::cout << " neighbours: ";
			for (int j : h.neighborhood(i)) std::cout << j << " ";
			std:: cout << std::endl;
		}
	}
	std::cout << "Co-tree: " << std::endl;
	for (int i : co_tree) std::cout << i << " ";
	std::cout << std::endl;

	Solver* s = new Solver(*g);
    Graph H(*g);

    SatSolver* ss = new SatSolver();
    std::vector<int> v;
    for (int i = 0; i < 10; i++) v.push_back(i);
    ss->add_auxiliary_constraint(v);
    delete ss;

	s->solve();
    auto w = g->width(s->get_contraction());

    std::cout << "Twin Width: " << w << std::endl;

    std::cout << "Done" << std::endl;

    delete g;
    delete s;
	return 0;
}
