#ifndef SRC_LINK_H_
#define SRC_LINK_H_

#include <string>
#include "Edge.h"

class Location;

class Link : public Edge<Location>{

	int id;
	std::string transport;

public:
	Link(Location *d, double w, int id);
	virtual ~Link();
	int getId();
};

#endif /* SRC_LINK_H_ */
