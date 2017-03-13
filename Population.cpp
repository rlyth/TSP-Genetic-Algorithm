// Population.cpp

#include "Population.hpp"

#include <math.h>
#include <cstdlib>
#include <iostream>
#include <numeric>

Population::Population(int pop_size, double elitism, double mutation) {
	this->populationSize = pop_size;
	this->elitism = elitism;
	this->mutation = mutation;
}

void Population::initialize(Tour seed) {
	// Set current mins to seed values in case getMins is called before updateMins
	minPath = seed.getID();
	minDist = seed.calcTourDist();
	
	for(int i = 0; i < populationSize; i++) {
		// Randomize order of location ids
		// We will still be able to access an ids corresponding x and y coords
		// Because its numerical value is also the index of its coordinates
		seed.shufflePath();
		
		// Add randomized tour to population
		currentPop.push_back(seed);
	}
}


Population::~Population() {};


void Population::nextGeneration() {
	std::vector<Tour> next_gen;
	
	// Obtain tour distance for all tours
	std::priority_queue<Evaluation> fitness_evals;
	evaluate(fitness_evals);
	
	// Select most optimal tours and add to next_gen
	// newEvals will contain the fitness values of the selected population
	std::vector<int> newEvals;
	newEvals = select(fitness_evals, next_gen);
	
	// Generate new tours from selected tours
	crossover(next_gen, newEvals);
	
	// Introduce random mutations
	mutate(next_gen);
	
	currentPop = next_gen;
}


void Population::evaluate(std::priority_queue<Evaluation> &eval) {
	for(int i = 0; i < populationSize; i++) {
		// Store tour index and calculated tour distance in struct
		Evaluation current_eval(i, currentPop.at(i).calcTourDist());
		
		// Push the evaluation struct into the priority queue
		eval.push(current_eval);
	}
}


std::vector<int> Population::select(std::priority_queue<Evaluation> &eval, std::vector<Tour> &next_gen) {
	int num_elitism = ceil(populationSize * elitism);
	std::vector<int> newEvals;
	
	// Selects the first num_elitism tours with the lowest total distances
	for(int i = 0; i < num_elitism; i++) {
		// Retrieve the tour located at the index stored in Eval struct
		Tour temp = currentPop.at(eval.top().tour_idx);
		
		next_gen.push_back(temp);
		newEvals.push_back(eval.top().fit_val);
		
		eval.pop();
	}
	
	return newEvals;
}


void Population::crossover(std::vector<Tour> &next_gen, std::vector<int> fit_evals) {
	int num_parents = ceil(populationSize * elitism);
	int num_children = populationSize - num_parents;
	
	// Used in roulette wheel selection
	//int totalFitness = std::accumulate(fit_evals.begin(), fit_evals.end(), 0);
	
	// Store tour size for easier reference
	int tour_size = next_gen.at(0).size();
	
	Tour childTour = next_gen.at(0);
	
	for(int i = 0; i < num_children; i += 2) {
		// Get the index for the crossover from range 1 .. (tour_size - 1)
		int cross = rand() % (tour_size - 2) + 1;
		
		// Get the index of two parents
		int parent1 = truncationSelect(num_parents);
		int parent2 = truncationSelect(num_parents);
		//int parent1 = rouletteSelect(next_gen, fit_evals, totalFitness);
		//int parent2 = rouletteSelect(next_gen, fit_evals, totalFitness);
		
		// Ensure parents are unique
		while(parent1 == parent2 && num_parents > 1) {
			parent2 = truncationSelect(num_parents);
			//parent2 = rouletteSelect(next_gen, fit_evals, totalFitness);
		}
		
		// First child
		std::vector<int> newPath = generateChild(next_gen.at(parent1).getID(),
			next_gen.at(parent2).getID(), cross);
		
		childTour.setID(newPath);
		
		next_gen.push_back(childTour);
		
		// Prevent extra children from being generated when num_children is odd
		if(next_gen.size() < populationSize) {
			// Second child
			newPath = generateChild(next_gen.at(parent2).getID(),
				next_gen.at(parent1).getID(), cross);
			
			childTour.setID(newPath);
			
			next_gen.push_back(childTour);
		}
	}
}


// Faster but less reliable
int Population::truncationSelect(int num_parents) {
	// Get a value in range 1 .. 100
	int wheelSpin = rand() % 100 + 1;
	
	// 50% chance to select parent from top third
	int p1 = ceil(num_parents/3);
	if(wheelSpin <= 50) {
		return rand() % p1;
	}
	
	// 33% chance to select parent from top 2/3
	int p2 = ceil(2 * num_parents / 3);
	if(wheelSpin <= 83) {
		return rand() % p2;
	}
	
	// 17% chance to select parent from entire population
	return rand() % num_parents;
}


// Slower but better, more consistent results
int Population::rouletteSelect(std::vector<Tour> next_gen, std::vector<int> fitEvals, int fitTotal) {
	int findParent = 0;
	
	// Generate random value in range 0 .. fitTotal
	int wheelSpin = rand() % fitTotal;
	
	// Iterate through population, summing fitnesses until wheelSpin is reached
	for(int i = 0; i < next_gen.size(); i++) {
		findParent += fitEvals[i];
		
		if(findParent >= wheelSpin) {
			return i;
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
	int tour_size = pop[0].size();
	
	// Guarantees at least one mutation per generation
	int num_mutations = ceil(populationSize * mutation * tour_size);
	
	for(int i = 0; i < num_mutations; i++) {
		// Get index of random solution
		// Range is from 1 .. pop.size() to preserve best solution in idx 0
		int to_mutate = rand() % (pop.size() - 1) + 1;
		
		// Get two indices from range 0 .. tour.size()
		int swap_from = rand() % tour_size;
		int swap_to = rand() % tour_size;
		
		// Discard duplicate indices
		while(swap_from == swap_to && tour_size > 1) {
			swap_to = rand() % tour_size;
		}
		
		pop[to_mutate].swap(swap_from, swap_to);
	}
}


void Population::updateMins() {
	std::priority_queue<Evaluation> eval;
	
	evaluate(eval);
	
	minDist = eval.top().fit_val;
	minPath = currentPop.at(eval.top().tour_idx).getID();
}


int Population::getMinDist() {
	return minDist;
}


std::vector<int> Population::getMinPath() {
	return minPath;
}


void Population::print() {
	for(int i = 0; i < currentPop.size(); i++) {
		std::cout << "Tour " << i << ": " << currentPop.at(i).calcTourDist() << "\n";
	}
}