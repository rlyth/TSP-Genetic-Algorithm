// tour.hpp

#ifndef TOUR_HPP
#define TOUR_HPP

#include "Coord.hpp"
#include <vector>

class Tour : public Coord {
	public:
		Tour();
		Tour(std::vector<int> ids, std::vector<int> x, std::vector<int> y);
		~Tour();
		int calcTourDist();
		void shufflePath();
		void swap(int i, int j);
		void setID(std::vector<int> newPath);
		std::vector<int> getID();
		std::vector<int> getX();
		std::vector<int> getY();
		void print();
};

#endif