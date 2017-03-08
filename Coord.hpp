#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>
#include <fstream>
#include <vector>

class Coord {

protected:
    std::vector<int> id,xc,yc;

public:
	Coord() {}; // constructor
	~Coord() {}; // destructor
	void readFile(const char * argv[]);
	void print();
	int n(int);
	int x(int);
	int y(int);
	int size();
	int dist(int,int);
};

#endif // COORD_HPP
