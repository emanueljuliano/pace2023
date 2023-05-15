#include <iostream>
#include <fstream>
#include <string>

#include "../include/graph.hpp"

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

    std::cout << "Done" << std::endl;

	return 0;
}
