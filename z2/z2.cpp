#include<vector>
#include<unordered_set>
#include<iostream>
#include<chrono>
#include<string>
#include<sstream>

#include"graph.hpp"

bool dfs(int64_t v1,
	const std::vector<std::unordered_set<int64_t>>& adj_list,
	std::vector<bool>& visited,
	std::vector<int64_t>& matching){

	if(visited[v1]) return false;
	visited[v1] = true;

	for(int64_t v2 : adj_list[v1]){
		if(matching[v2] == -1 || dfs(matching[v2], adj_list, visited, matching)){
			matching[v2] = v1;
			return true;
		}
	}
	return false;
}

std::pair<std::size_t, std::vector<int64_t>> maximum_matching(const RandomBipartileGraph& graph){
	//Kuhn's algorithm

	const auto&adj_list = graph.get_adj_list();
	const std::size_t num_vertices = graph.get_num_vertices();

	std::vector<int64_t> matching(num_vertices, -1);
	std::vector<bool> visited(num_vertices, false);

	std::size_t match_count = 0;
	std::size_t V1_num_vertices = adj_list.size();
	for(int64_t v = 0; v < V1_num_vertices; ++v){
		visited.assign(num_vertices, false);
		if(dfs(v, adj_list, visited, matching)){
			++match_count;
		}
	}

	return {match_count, matching};
}


int main(int argc, char*argv[]){
	if(argc < 5 || std::string(argv[1]) != "--size" || std::string(argv[3]) != "--degree"){
		std::cerr << "usage: z1 --size <k> --degree <i>" << std::endl;
		return 1;
	}

	const bool print_matching = (argc == 6 && std::string(argv[5]) == "--printMatching");

	const std::size_t k = std::stoi(argv[2]);
	const std::size_t i = std::stoi(argv[4]);

	RandomBipartileGraph graph(k, i);

	graph.print_graph();

	auto [match_count, matching] = maximum_matching(graph);

	if(print_matching){
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(nullptr);

		std::ostringstream buffer;

		for(int64_t u = 0; u < matching.size(); ++u){
			if(matching[u] == -1) continue;
			buffer << "(" << u << ", " << matching[u] << ")" << "\n";
		}

		std::cout << buffer.str();
	}

	return 0;
}
