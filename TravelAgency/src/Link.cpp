#include "Link.h"

Link::Link(Location *d, double w) : Edge<Location>(d,w){}


Link::~Link() {}


int Link::getId(){
	return id;
}
