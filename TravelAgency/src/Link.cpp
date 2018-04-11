#include "Link.h"

Link::Link(Location *d, double w, int id, std::string name) : Edge<Location>(d,w), id(id), name(name){}


Link::~Link() {}

const std::string& Link::getName() const {
	return name;
}

int Link::getId(){
	return id;
}
