/*********************************************************************
  ** Program Filename: TSP
  ** Author: R Thompson
  ** Date: 03/07/17
  ** Description: Main function to control a genetic algorithm for the
		traveling salesman problem
  ** Input: A properly-formatted text file with IDs and x-y coordinates
		for a graph
  ** Output: The most optimal solution found by the algorithm, written
		to [input-filename].tour
  *********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

#include "Tour.hpp"
#include "Population.hpp"

int main(int argc, const char * argv[]) {
	// Genetic Parameters
	int population_size = 500;
	float elitism_rate = .25;
	float mutation_rate = .05;
	int generations = 1000;
	
	std::srand(std::time(NULL));
	
    Tour initial_tour;
    
    // Read coordinates from file
    initial_tour.readFile(argv);
	
	Population pop(population_size, elitism_rate, mutation_rate);
	
	// Randomly generates population_size many shuffled tours
	pop.initialize(initial_tour);
	
	// Complete generations many lifecycles
	for(int i = 0; i < generations; i++) {
		pop.next_generation();
	}
	
	// Retrieve the most optimal solution from the final population
	pop.update_mins();
	std::vector<int> minPath = pop.getMinPath();
	int minDist = pop.getMinDist();
	
	// Filename for output file
	std::string filename(argv[1]);
	filename = filename + ".tour";
	
	std::ofstream outFile;
	outFile.open(filename.c_str(), std::ios::trunc);
	
	// Write minimum tour distance and path to file
	outFile << minDist << "\n";
	for(int i = 0; i < minPath.size(); i++) {
		outFile << minPath.at(i) << "\n";
	}
	outFile.close();

    return 0;
}
