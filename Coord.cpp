// Author: Diana Bacon
// Coordinate class implementation
// 2017-03-06

#include <math.h>
#include "Coord.hpp"
void Coord::readFile(const char * argv[])
// reads coordinate file
{
    int n,x,y;
    std::ifstream inputFile;
    
    inputFile.open(argv[1]);
    if (inputFile.fail()) {
        std::cout << "Error opening data file.\n";
    } else {
        
        while (inputFile >> n >> x >> y) {
            id.push_back(n);
            xc.push_back(x);
            yc.push_back(y);
        }
        
        inputFile.close();
    }
}

void Coord::print() {
	for(int i=0; i<id.size(); i++) {
		std::cout << id.at(i) << "\t" << xc.at(i) << "\t" << yc.at(i) << "\n";
	}
}

int Coord::n(int i) {
	return id.at(i);
}

int Coord::x(int i) {
	return xc.at(i);
}

int Coord::y(int i) {
	return yc.at(i);
}

int Coord::size() {
	return id.size();
}

int Coord::dist(int i, int j) {
	return round( sqrt( pow(xc.at(i)-xc.at(j),2) +
						pow(yc.at(i)-yc.at(j),2) ) );
}