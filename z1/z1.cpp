#include<unordered_map>
#include<queue>
#include<vector>
#include<numeric>
#include<chrono>
#include<iostream>
#include<string>
#include<utility>
#include<sstream>
#include<fstream>

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

void generate_max_flow_model(const Graph& graph, int64_t s, 
			int64_t t, const std::string& filename){
	const std::size_t num_nodes = graph.get_num_nodes();
	const auto& adj_list = graph.get_adj_list();

	std::ofstream out(filename);
	if(!out){
		return;
	}

	out << "using JuMP\n";
	out << "using GLPK\n\n";

	out << "model = Model(GLPK.Optimizer)\n\n";

	out << "#edges and capacities\n";
	out << "edges = [";
	bool first_edge = true;
	for(int64_t u = 0; u < num_nodes; ++u){
		for(const auto&edge : adj_list[u]){
			if(!first_edge){
				out << ", ";
			}
			out << "(" << u + 1 << ", " << edge.to + 1 << ")";
			first_edge = false;
		}
	}
	out << "]\n";

	out << "capacities = Dict{Tuple{Int64, Int64}, Int64}(\n";
	first_edge = true;
	for(int64_t u = 0; u < num_nodes; ++u){
		for(const auto& edge : adj_list[u]){
			if(!first_edge){
				out << ",\n";
			}
			out << "\t(" << u + 1 << ", " << edge.to + 1 << ") => " << edge.capacity;
			first_edge = false;
		}
	}
	out << ")\n\n";

	out << "#decision variables\n";
	out << "@variable(model, flow[edges] >= 0)\n\n";

	out << "#capacity constraints\n";
	out << "for e in edges\n";
	out << "	@constraint(model, flow[e] <= capacities[e])\n";
	out << "end\n\n";

	out << "#flow conservation constraints\n";
	out << "for u in 1:" << num_nodes << "\n";
	out << "	if u == " << s + 1 << " || " << "u == " << t + 1 << "\n";
	out << "		continue\n";
	out << "	end\n";
	out << "	@constraint(model, sum(flow[(u,v)] for v in 1:" << num_nodes
		<< " if (u, v) in edges) == sum(flow[(v,u)] for v in 1:" << num_nodes
		<< " if (v, u) in edges))\n";
	out << "end\n";
	

	out << "\n";

	out << "#objective: maximize flow from source to sink\n";
	out << "@objective(model, Max, sum(flow[(" << s + 1 
		<< ", v)] for v in 1:" << num_nodes << " if (" << s + 1 
		<< ", v) in edges))\n\n";

	out << "optimize!(model)\n\n";

	out << "#print the results\n";
	out << "if termination_status(model) == MOI.OPTIMAL\n";
	out << "	println(\"maximum flow: \", objective_value(model))\n";
	out << "	for e in edges\n";
	out << "		println(\"flow[\", e, \"] = \", value(flow[e]))\n";
	out << "	end\n";
	out << "else\n";
	out << "	println(\"no optimal solution\")\n";
	out << "end\n";

	out.close();
}

int main(int argc, char*argv[]){
	if(argc < 3 || std::string(argv[1]) != "--size"){
		std::cerr << "usage: z1 --size <cube dimension> --printFlow" <<std::endl;
		return 1;
	}

	const std::size_t k = std::stoi(argv[2]);
	const bool print_flow = (argc >= 4 && std::string(argv[3]) == "--printFlow");

	Graph graph(k);

	//glpk file
	const bool gen_glpk = ((argc >= 5 && std::string(argv[4]) == "--glpk") ||
			(argc >= 4 && std::string(argv[3]) == "--glpk"));
	std::string glpk_filename;
	if(gen_glpk){
		if(argc == 6)
			glpk_filename = std::string(argv[5]);
		else if(argc == 5)
			glpk_filename = std::string(argv[4]);

		generate_max_flow_model(graph, 0, (1 << k) - 1, glpk_filename);
	}


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
					buffer << u + 1 << "\t" << v + 1<< "\t" << 
						flow[u][v] << "\n";
				}
			}
		}

		std::cout << buffer.str();
	}

	return 0;
}
