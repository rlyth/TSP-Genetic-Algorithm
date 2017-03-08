// tour.cpp

#include "Tour.hpp"

#include <iostream>
#include <algorithm>

Tour::Tour() {}

Tour::Tour(std::vector<int> ids, std::vector<int> x, std::vector<int> y) {
	id = ids;
	xc = x;
	yc = y;
}

Tour::~Tour() {}


int Tour::calcTourDist() {
	int total_dist = 0;
	int length = id.size();
	
	for(int i = 0; i < length; i++) {
		if(i == length - 1) {
			// For the last element in array, compute distance to first element
			// dist() is passed the values at the indices, as the values correspond
			// to the indices where that location's x and y coordinates are stored
			total_dist += dist(id.at(i), id.at(0));
		}
		else {
			// Otherwise compute distance from element to next element
			total_dist += dist(id.at(i), id.at(i+1));
		}
	}
	
	return total_dist;
}


void Tour::shufflePath() {
	std::random_shuffle(id.begin(), id.end());
}


void Tour::swap(int i, int j) {
	// Store the value at i
	int temp = id.at(i);
	
	// Update the value at i to the value at j
	id[i] = id.at(j);
	
	// Update the value at j to the value that was at i
	id[j] = temp;
}


void Tour::setID(std::vector<int> newPath) {
	id = newPath;
}


std::vector<int> Tour::getID() {
	return id;
}

std::vector<int> Tour::getX() {
	return xc;
}

std::vector<int> Tour::getY() {
	return yc;
}


void Tour::print() {
	for(int i=0; i < id.size(); i++) {
		// The value at i (is also the index of the x and y coords)
		int loc = id.at(i);
		
		std::cout << loc << "\t" << xc.at(loc) << "\t" << yc.at(loc) << "\n";
	}
}