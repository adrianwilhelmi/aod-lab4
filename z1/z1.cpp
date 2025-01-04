#include<unordered_map>
#include<queue>
#include<vector>
#include<numeric>
#include<chrono>
#include<iostream>
#include<string>
#include<utility>
#include<sstream>

#include"graph.hpp"

std::pair<int64_t, std::vector<std::unordered_map<int64_t, int64_t>>> edmonds_karp(const Graph& graph, int64_t s, int64_t t, bool print_flow = false){
	auto start_time = std::chrono::high_resolution_clock::now();

	const std::size_t num_nodes = graph.get_num_nodes();
	const auto&adj_list = graph.get_adj_list();


	std::vector<std::unordered_map<int64_t, int64_t>> capacity(num_nodes);
	std::vector<std::unordered_map<int64_t, int64_t>> flow(num_nodes);

	for(int64_t u = 0; u < num_nodes; ++u){
		for(const auto&edge : adj_list[u]){
			capacity[u][edge.to] = edge.capacity;
		}
	}

	int64_t total_flow = 0;
	std::size_t augmenting_paths_count = 0;

	while(true){
		std::vector<int64_t> parent(num_nodes, -1);
		std::queue<int64_t> q;
		q.push(s);
		parent[s] = s;

		while(!q.empty() && parent[t] == -1){
			int64_t current = q.front();
			q.pop();

			for(const auto& [next, cap] : capacity[current]){
				if(parent[next] == -1 && 
				cap > flow[current][next]){
					parent[next] = current;
					q.push(next);
				}
			}
		}

		if(parent[t] == -1) break;

		++augmenting_paths_count;

		int64_t path_flow = std::numeric_limits<int64_t>::max();
		for(int64_t v = t; v != s; v = parent[v]){
			int64_t u = parent[v];
			path_flow = std::min(path_flow, capacity[u][v] - flow[u][v]);
		}

		for(int64_t v = t; v != s; v = parent[v]){
			int64_t u = parent[v];
			flow[u][v] += path_flow;
			flow[v][u] -= path_flow;
		}

		total_flow += path_flow;
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();

	std::cout << total_flow << std::endl;
	std::cerr << elapsed_time << "\t" << augmenting_paths_count << std::endl;

	return {total_flow, flow};
}

int main(int argc, char*argv[]){
	if(argc < 3 || std::string(argv[1]) != "--size"){
		std::cerr << "usage: z1 --size <cube dimension> --printFlow" <<std::endl;
		return 1;
	}

	const std::size_t k = std::stoi(argv[2]);
	const bool print_flow = (argc == 4 && std::string(argv[3]) == "--printFlow");

	Graph graph(k);

	auto [max_flow, flow] = edmonds_karp(graph, 0, (1 << k) - 1, print_flow);

	//std::cout << "max flow: " << max_flow << std::endl;

	if(print_flow){
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(nullptr);

		std::ostringstream buffer;

		const auto&adj_list = graph.get_adj_list();
		const std::size_t num_nodes = graph.get_num_nodes();

		for(int64_t u = 0; u < num_nodes; ++u){
			for(const auto&edge : adj_list[u]){
				int64_t v = edge.to;
				if(flow[u][v] > 0){
					buffer << u << "\t" << v << "\t" << 
						flow[u][v] << "\n";
				}
			}
		}

		std::cout << buffer.str();
	}

	return 0;
}
