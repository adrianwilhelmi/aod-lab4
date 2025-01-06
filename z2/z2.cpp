#include<vector>
#include<unordered_set>
#include<iostream>
#include<chrono>
#include<string>
#include<sstream>
#include<fstream>

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

void generate_matching_model(const RandomBipartileGraph& graph, const std::string& filename){
	const auto& adj_list = graph.get_adj_list();
	const std::size_t n1 = adj_list.size();
	const std::size_t n2 = graph.get_num_vertices() - n1;

	std::ofstream out(filename);
	if(!out)
		return;

	out << "using JuMP\nusing GLPK\n\n";
	out << "model = Model(GLPK.Optimizer)\n\n";

	out << "# edges in the bipartile graph\n";
	out << "E = [";
	bool first = true;
	for(std::size_t u = 0; u < n1; ++u){
		for(const auto& v : adj_list[u]){
			if(!first){
				out << ", ";
			}
			out << "(" << u + 1 << ", " << v + 1 + n1 << ")";
			first = false;
		}
	}
	out << "]\n\n";

	out << "# binary decision variables\n";
	out << "@variable(model, x[E], Bin)\n\n";

	out << "#define objective function: maximize sum of x for each edge in E\n";
	out << "@objective(model, Max, sum(x[e] for e in E))\n";

	out << "#constraint: each vertex in V1 can be part of at most one edge\n";
	out << "for u in 1:" << n1 << "\n";
	out << "	@constraint(model, sum(x[(u,v)] for v in " << n1 + 1 << ":" << n1 + n2 << " if (u,v) in E) <= 1)\n";
	out << "end\n\n";

	out << "#constraint: each vertex in v2 can be part of at most one edge\n";
	out << "for v in " << n1 + 1 << ":" << n1 + n2 << "\n";
	out << "	@constraint(model, sum(x[(u,v)] for u in 1:" << n1 << " if (u,v) in E) <= 1)\n";
	out << "end\n\n";

	out << "#optimize the model\n";
	out << "optimize!(model)\n\n";

	out << "#print the values of the decision variables\n";
	out << "for e in E\n";
	out << "	println(\"x[\", e, \"] = \", value(x[e]))\n";
	out << "end\n\n";

	out << "# save the model to file\n";
	out << "write_to_file(model, \"" << filename << ".lp\")\n";

	out.close();
}	

int main(int argc, char*argv[]){
	if(argc < 5 || std::string(argv[1]) != "--size" || std::string(argv[3]) != "--degree"){
		std::cerr << "usage: z1 --size <k> --degree <i>" << std::endl;
		return 1;
	}

	const bool print_matching = (argc >= 6 && std::string(argv[5]) == "--printMatching");

	const std::size_t k = std::stoi(argv[2]);
	const std::size_t i = std::stoi(argv[4]);

	RandomBipartileGraph graph(k, i);


	//glpk file
	const bool gen_glpk = ((argc >= 7 && std::string(argv[6]) == "--glpk") ||
			(argc >= 6 && std::string(argv[5]) == "--glpk"));
	std::string glpk_filename;
	if(gen_glpk){
		if(argc == 8)
			glpk_filename = std::string(argv[7]);
		else if(argc == 7)
			glpk_filename = std::string(argv[6]);

		generate_matching_model(graph, glpk_filename);
	}


	//graph.print_graph();

	auto start = std::chrono::high_resolution_clock::now();

	auto [match_count, matching] = maximum_matching(graph);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << elapsed.count() << std::endl;
	std::cout << match_count << std::endl;

	if(print_matching){
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(nullptr);

		std::ostringstream buffer;

		std::size_t V2_size = graph.get_num_vertices() / 2;
		for(int64_t u = 0; u < V2_size; ++u){
			if(matching[u] == -1) continue;
			buffer << "(" << 1 + u << ", " << 1 + V2_size + matching[u] << ")" << "\n";
		}

		std::cout << buffer.str();
	}

	return 0;
}
