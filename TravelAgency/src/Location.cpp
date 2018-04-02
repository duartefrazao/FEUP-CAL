#include "Location.h"
#include "iostream"
#include "Vertex.h"


Location::Location(int id, double x, double y, std::string name)
	: id(id), x(x), y(y), name(name){}


Location::~Location() {}

bool Location::operator==(const Location &l){
	return this->id == l.id;
}

int Location::getId() const {
		return id;
}

double Location::distance(Location *l){


	double vx = (this->x - l->x);
	double vy = (this->y - l->y);
	double res = sqrt(vx*vx + vy*vy);
	return res;
}

void Location::addEdge(Location *d, double w) {
	adj.push_back(Link(d, w));
}
