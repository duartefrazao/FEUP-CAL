#include "Link.h"

Link::Link(Location *d, double w, int id) : Edge<Location>(d,w), id(id){}


Link::~Link() {}


int Link::getId(){
	return id;
}
