#include "Location.h"
#include "iostream"
#include "Vertex.h"


Location::Location(unsigned long int id, int x, int y, double lat, double lon, std::string name)
	: id(id), x(x), y(y), lat(lat), lon(lon) , name(name){}


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

void Location::addEdge(Location *d, double w, unsigned long int id) {
	adj.push_back(Link(d, w, id));
}

double Location::getX() const {
	return x;
}

bool Location::operator<(const Location& l) {
	return this->f < l.f;
}

double Location::getY() const {
	return y;
}

