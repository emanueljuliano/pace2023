#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> read_graph(string& filename) {
	ifstream in(filename);

	string s;	
	in >> s >> s;
	
	int n, m;
	in >> n >> m;

	vector graph(n, vector<int>(n));

	for (int i = 0; i < m; i++) {
		int a, b;
		in >> a >> b;
		a--, b--;
		graph[a][b] = graph[b][a] = 1;
	}

	return graph;
}

vector<pair<int, int>> read_contraction_sequence(string filename, int length) {
	vector<pair<int, int>> sequence(length);

	ifstream in(filename);
	for (int i = 0; i < length; i++) {
		in >> sequence[i].first >> sequence[i].second;	
		sequence[i].first--;
		sequence[i].second--;
	}

	return sequence;
}

void contract(int a, int b, vector<vector<int>> &graph, int &max_degree) {
	int n = graph.size();
	
	for (int i = 0; i < n; i++) if (i != a and i != b) {
		if (graph[a][i] and graph[b][i]) { // intersection
			if (!(graph[a][i] == 1 and graph[b][i] == 1)) {
				graph[a][i] = graph[i][a] = -1;
			}
		}
		else if (graph[a][i] or graph[b][i]) {
			graph[a][i] = graph[i][a] = -1;
		}
	}

	for (int i = 0; i < n; i++) graph[i][b] = graph[b][i] = 0;
	
	for (int i = 0; i < n; i++) {
		int at = 0;
		for (int j = 0; j < n; j++) at += graph[i][j] == -1;
		max_degree = max(max_degree, at);
	}
}

int tww(vector<vector<int>> graph, vector<pair<int, int>> contr) {
	int max_degree = 0;
	for (auto [a, b] : contr) {
		contract(a, b, graph, max_degree);
	}
	return max_degree;
}

int main(int argc, char** argv) {
	if (argc <= 2)
		return 1;

	string input_file = argv[1];
	string output_file = argv[2];

	auto graph = read_graph(input_file);

	int n = graph.size();
	auto sequence = read_contraction_sequence(output_file, n - 1);

	cout << "Twin Width: " << tww(graph, sequence) << endl;

	return 0;
}
