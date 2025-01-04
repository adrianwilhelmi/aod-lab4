#include<vector>
#include<random>
#include<unordered_set>
#include<iostream>

class RandomBipartileGraph{
private:
	std::vector<std::unordered_set<int64_t>> V1_adj_list;
	std::size_t k, i;
	std::size_t V1_num_vertices;

public:
	RandomBipartileGraph(std::size_t k, std::size_t i) : k(k), i(i){
		this->V1_num_vertices = (1 << k);
		V1_adj_list.resize(this->V1_num_vertices);

		std::random_device rd;
		std::mt19937 rng(rd());

		std::uniform_int_distribution<int64_t> dist(0, 
				this->V1_num_vertices - 1);

		for(int64_t v = 0; v < V1_num_vertices; ++v){
			std::unordered_set<int64_t> neighbors;
			while(neighbors.size() < i){
				int64_t neighbor = dist(rng);
				neighbors.insert(neighbor);
			}
			V1_adj_list[v] = std::move(neighbors);
		}
	}

	void print_graph() const{
		for(std::size_t v = 0; v < V1_num_vertices; ++v){
			std::cout << "V1(" << v << ") -> ";
			for(int64_t neighbor : V1_adj_list[v]){
				std::cout << "V2(" << neighbor << ") ";
			}
			std::cout << std::endl;
		}
	}

	const std::size_t get_num_vertices() const{
		return V1_num_vertices;
	}

	const std::unordered_set<int64_t>& get_neighbors(int64_t v) const{
		return V1_adj_list[v];
	}
};

