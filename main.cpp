/*********************************************************************
  ** Program Filename: TSP
  ** Author: R Thompson
  ** Date: 03/07/17
  ** Description: Main function to control a genetic algorithm for the
		traveling salesman problem
  ** Input: A properly-formatted text file with IDs and x-y coordinates
		for a graph, separated by spaces, one ID and coord set per line
  ** Output: The most optimal solution found by the algorithm, written
		to [input-filename].tour
  *********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <limits>

#include "Tour.hpp"
#include "Population.hpp"

void validateInput(int min, int &target);
void validateInput(double min, double &target, double max);

int main(int argc, const char * argv[]) {
	// Genetic Parameters
	int population_size = 100;
	double elitism_rate = .20;
	double mutation_rate = .05;
	int generations = 2000;
	
	if(argc < 2) {
		std::cout << "Usage is 'tspGA input-filename.txt'\n";
		
		return 1;
	}
	
	std::srand(std::time(NULL));
	
    Tour initial_tour;
    
    // Read coordinates from file
    initial_tour.readFile(argv);
	
	// Menu options and selection
	std::cout << "\n--- TSP Genetic Algorithm ---\n";
	std::cout << "-------------------------------\n";
	std::cout << "Current Parameters\n";
	std::cout << "Population Size: " << population_size << "\n";
	std::cout << "Elitism Rate: " << elitism_rate << "\n";
	std::cout << "Mutation Rate: " << mutation_rate << "\n";
	std::cout << "Generations: " << generations << "\n";
	
	std::cout << "\n1. Run algorithm with these parameters\n";
	std::cout << "2. Edit parameters\n";
	std::cout << "3. Quit\n";
	std::cout << "> ";
	
	int choice;
	validateInput(1, choice);
	
	// Deal with special options, otherwise run with default values
	if(choice == 3) {
		return 0;
	}
	else if(choice == 2) {
		std::cout << "\nPopulation Size> ";
		validateInput(2, population_size);
		
		std::cout << "Elitism Rate> ";
		validateInput(0.0001, elitism_rate, 1);
		
		std::cout << "Mutation Rate> ";
		validateInput(0, mutation_rate, 1);
		
		std::cout << "Generations> ";
		validateInput(1, generations);
	}
	
	// Algorithm start
	Population pop(population_size, elitism_rate, mutation_rate);
	
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


void validateInput(int min, int &target) {
	using std::endl;
	using std::cout;
	using std::cin;
	
	while(!(cin >> target) || target < min) {
		cout << "Invalid input. Please enter an integer above " <<
			(min - 1) << "." << endl << "> ";
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void validateInput(double min, double &target, double max) {
	using std::endl;
	using std::cout;
	using std::cin;
	
	while(!(cin >> target) || target < min || target > max) {
		cout << "Invalid input. Please enter a number from " <<
			min << " to " << max << "." << endl << "> ";
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}