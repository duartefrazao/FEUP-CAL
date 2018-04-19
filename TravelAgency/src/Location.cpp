#include "Location.h"
#include "iostream"
#include "Vertex.h"

Location::Location(unsigned long int id, double lat, double lon,
		std::string name) :
		id(id), x { 0 }, y { 0 }, lat(lat), lon(lon), name(name) {
}

Location::Location(unsigned long int id, int x, int y, std::string name) :
		id(id), x(x), y(y), lat(0), lon(0), name(name) {
}

Location::~Location() {
}

bool Location::operator==(const Location &l) {
	return this->id == l.id;
}

int Location::getId() const {
	return id;
}

double Location::distance(Location *l) {
	/*
	 double vx = (this->x - l->x);
	 double vy = (this->y - l->y);
	 double res = sqrt(vx*vx + vy*vy);
	 return res;
	 */
	double vx = (this->lat - l->lat);
	double vy = (this->lon - l->lon);
	double res = sqrt(vx * vx + vy * vy);
	return res;

}

void Location::addEdge(Location *d, double w, unsigned long int id,
		std::string string) {
	adj.push_back(Link(d, w, id, string));
}

double Location::getX() const {
	return x;
}

void Location::setX(double x) {
	this->x = x;
}

void Location::setY(double y) {
	this->y = y;
}

bool Location::operator<(const Location& l) {
	return this->f < l.f;
}

double Location::getY() const {
	return y;
}

double Location::getLat() const {
	return lat;
}

double Location::getLon() const {
	return lon;
}
