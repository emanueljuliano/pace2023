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

int main(int argc, char* argv[]) {
    if (argc <= 1)
        return 1;

    std::string input_file = argv[1];

    Graph* g = read_graph(input_file);
    Solver* s = new Solver(*g);

    s->solve();
    s->print_contraction();
    auto w = g->width(s->get_contraction());

    std::cout << "Kaio's Width: " << w << std::endl;

    std::cout << "Done" << std::endl;

    delete g;
    delete s;
	return 0;
}
