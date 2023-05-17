#include <iostream>
#include <fstream>
#include <string>

#include "../include/graph.hpp"
#include "../include/solver.hpp"

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
    Solver* s = new Solver(*g);
    Graph H(*g);

    s->solve();

    std::cout << "Done" << std::endl;

    delete g;
    delete s;
	return 0;
}
