// Population.cpp

#include "Population.hpp"

#include <math.h>
#include <cstdlib>
#include <iostream>

Population::Population(int pop_size, double elitism, double mutation) {
	this->population_size = pop_size;
	this->elitism = elitism;
	this->mutation = mutation;
}

void Population::initialize(Tour seed) {
	// Set current mins to seed values in case getMins is called before updateMins
	min_path = seed.getID();
	min_dist = seed.calcTourDist();
	
	for(int i = 0; i < population_size; i++) {
		// Randomize order of location ids
		// We will still be able to access an ids corresponding x and y coords
		// Because its numerical value is also the index of its coordinates
		seed.shufflePath();
		
		// Add randomized tour to population
		current_pop.push_back(seed);
	}
}


Population::~Population() {};


void Population::next_generation() {
	std::vector<Tour> next_gen;
	
	// Obtain tour distance for all tours
	std::priority_queue<Evaluation> fitness_evals;
	evaluate(fitness_evals);
	
	// Select most optimal tours and add to next_gen
	select(fitness_evals, next_gen);
	
	// Generate new tours from selected tours
	crossover(next_gen);
	
	// Introduce random mutations
	mutate(next_gen);
	
	current_pop = next_gen;
}


void Population::evaluate(std::priority_queue<Evaluation> &eval) {
	for(int i = 0; i < population_size; i++) {
		// Store tour index and calculated tour distance in struct
		Evaluation current_eval(i, current_pop.at(i).calcTourDist());
		
		// Push the evaluation struct into the priority queue
		eval.push(current_eval);
	}
}


void Population::select(std::priority_queue<Evaluation> &eval, std::vector<Tour> &next_gen) {
	int num_elitism = ceil(population_size * elitism);
	
	// Selects the first num_elitism tours with the lowest total distances
	for(int i = 0; i < num_elitism; i++) {
		// Retrieve the tour located at the index stored in Eval struct
		Tour temp = current_pop.at(eval.top().tour_idx);
		
		next_gen.push_back(temp);
		
		eval.pop();
	}
}


void Population::crossover(std::vector<Tour> &next_gen) {
	int num_parents = ceil(population_size * elitism);
	int num_children = population_size - num_parents;
	
	// Store tour size for easier reference
	int tour_size = next_gen.at(0).size();
	
	Tour childTour = next_gen.at(0);
	
	for(int i = 0; i < num_children; i += 2) {
		// Get the index for the crossover from range 1 .. (tour_size - 1)
		int cross = rand() % (tour_size - 2) + 1;
		
		// Get the index of two parents
		int parent1 = rand() % num_parents;
		int parent2 = rand() % num_parents;
		
		// Ensure parents are unique
		while(parent1 == parent2 && num_parents > 1) {
			parent2 = rand() % num_parents;
		}
		
		// First child
		std::vector<int> newPath = generateChild(next_gen.at(parent1).getID(),
			next_gen.at(parent2).getID(), cross);
		
		childTour.setID(newPath);
		
		next_gen.push_back(childTour);
		
		// Prevent extra children from being generated when num_children is odd
		if(next_gen.size() < population_size) {
			newPath = generateChild(next_gen.at(parent2).getID(),
				next_gen.at(parent1).getID(), cross);
			
			childTour.setID(newPath);
			
			next_gen.push_back(childTour);
		}
	}
}


std::vector<int> Population::generateChild(std::vector<int> path1, std::vector<int> path2, int cross) {
	std::vector<int> childPath;
	int tour_size = path1.size();
	
	// Array to track added ids; 0 = not added, 1 = added
	int added_tracker[tour_size];
	for(int i = 0; i < tour_size; i++) { added_tracker[i] = 0; }
	
	// Inherit from parent 1
	for(int i = 0; i < cross; i++) {
		// Copy id from p1
		childPath.push_back(path1.at(i));
		
		// Update tracker
		added_tracker[path1.at(i)] = 1;
	}
	
	// Inherit from parent 2
	for(int i = cross; i < path1.size(); i++) {
		// If id is not yet added to child
		if(added_tracker[path2.at(i)] == 0) {
			// Add id to child
			childPath.push_back(path2.at(i));
			
			// Update tracker
			added_tracker[path2.at(i)] = 1;
		}
		else {
			int j = 0;
			
			// Search first half of path2 for available id
			while(added_tracker[path2.at(j)] != 0) {
				j++;
			}
			childPath.push_back(path2.at(j));
			
			// Update tracker
			added_tracker[path2.at(j)] = 1;
		}
	}
	
	return childPath;
}


void Population::mutate(std::vector<Tour> &pop) {	
	// Guarantees at least one mutation per generation
	int num_mutations = ceil(population_size * mutation);
	
	for(int i = 0; i < num_mutations; i++) {
		// Get index of random solution
		// Range is from 1 .. pop.size() to preserve best solution in idx 0
		int to_mutate = rand() % (pop.size() - 1) + 1;
		
		// Get two indices from range 0 .. tour.size()
		int swap_from = rand() % pop[to_mutate].size();
		int swap_to = rand() % pop[to_mutate].size();
		
		// Discard duplicate indices
		while(swap_from == swap_to && pop[to_mutate].size() > 1) {
			swap_to = rand() % pop[to_mutate].size();
		}
		
		pop[to_mutate].swap(swap_from, swap_to);
	}
}


void Population::update_mins() {
	std::priority_queue<Evaluation> eval;
	
	evaluate(eval);
	
	min_dist = eval.top().fit_val;
	min_path = current_pop.at(eval.top().tour_idx).getID();
}


int Population::getMinDist() {
	return min_dist;
}


std::vector<int> Population::getMinPath() {
	return min_path;
}


void Population::print() {
	for(int i = 0; i < current_pop.size(); i++) {
		std::cout << "Tour " << i << ": " << current_pop.at(i).calcTourDist() << "\n";
	}
}