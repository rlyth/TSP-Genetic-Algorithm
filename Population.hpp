// Population.hpp

#ifndef POPULATION_HPP
#define POPULATION_HPP

#include "Tour.hpp"
#include <vector>
#include <queue>

// Stores tour index with its fitness evaluation, used in a priority queue
struct Evaluation {
	int tour_idx;
	int fit_val;
	explicit Evaluation(int id, int fit) : tour_idx(id), fit_val(fit) {}
	// These operators are reversed so that lower distances have higher priority
	bool operator<( const Evaluation & e ) const {
		return fit_val > e.fit_val;
	}
	bool operator>( const Evaluation & e ) const {
		return fit_val < e.fit_val;
	}
};

class Population {
	private:
		std::vector<Tour> current_pop;
		int population_size;
		double elitism;
		double mutation;
		int min_dist;
		std::vector<int> min_path;

	public:
		Population(int pop_size, double elitism_rate, double mutation_rate);
		~Population();
		void initialize(Tour seed);
		void next_generation();
		void update_mins();
		int getMinDist();
		std::vector<int> getMinPath();
		void print();
		
	private:
		void evaluate(std::priority_queue<Evaluation> &eval);
		void select(std::priority_queue<Evaluation> &eval, std::vector<Tour> &pop);
		void crossover(std::vector<Tour> &next_gen);
		void mutate(std::vector<Tour> &pop);
		std::vector<int> generateChild(std::vector<int> p1, std::vector<int> p2, int cross);
};


#endif