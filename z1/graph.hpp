#include<vector>
#include<iostream>
#include<cmath>
#include<bitset>
#include<random>
#include<algorithm>

struct Edge{
	int64_t to;
	int64_t capacity;
};

class Graph{
public:
	int64_t k;
	std::size_t num_nodes;
	std::size_t num_edges;
	std::vector<std::vector<Edge>> adj_list;

	Graph(int64_t k) : k(k)
	{
		num_nodes = (1 << k);
		num_edges = k * (1 << (k - 1));
		adj_list.resize(num_nodes);

		std::random_device rd;
		std::mt19937 gen(rd());

		for(int64_t i = 0; i < this->num_nodes; ++i){
			for(int64_t j = 0; j < k; ++j){
				int64_t neighbor = i ^ (1 << j);

				if(neighbor <= i)
					continue;

				if(neighbor >= this->num_nodes){
					std::cout << "sasiad poza zakresem " << i << std::endl;
					continue;
				}

				int64_t l = calculate_l(i, neighbor);
				std::uniform_int_distribution<int64_t> dist(1, (1 <<l));

				int64_t capacity = dist(gen);
				this->adj_list[i].push_back({neighbor, capacity});
			}
		}
	}

	void print_adj_list(){
		for(std::size_t i = 0; i < num_nodes; ++i){
			for(auto&neighbor : adj_list[i]){
				std::cout << i << " -> " << neighbor.to << 
					": " << neighbor.capacity << std::endl;
			}
		}
	}

	const std::size_t get_num_nodes() const{
		return this->num_nodes;
	}

	const std::size_t get_num_edges() const{
		return this->num_edges;
	}

	std::vector<std::vector<Edge>>& get_adj_list(){
		return adj_list;
	}


	const std::vector<std::vector<Edge>>& get_adj_list() const{
		return adj_list;
	}

private:
	static int64_t hamming_weight(const int64_t x){
		return std::bitset<64>(x).count();
	}

	int64_t calculate_l(int64_t i, int64_t j){
		int64_t hi = hamming_weight(i);
		int64_t hj = hamming_weight(j);
		int64_t zi = k - hi;
		int64_t zj = k - hj;

		int64_t values[] = {hi, hj, zi, zj};

		return *std::max_element(
			std::begin(values), std::end(values)
		);
	}
};
