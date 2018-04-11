#ifndef SRC_LINK_H_
#define SRC_LINK_H_

#include <string>
#include "Edge.h"

class Location;

class Link : public Edge<Location>{

	int id;
	std::string name;

public:
	Link(Location *d, double w, int id,std::string name);
	virtual ~Link();
	int getId();
	const std::string& getName() const;
};

#endif /* SRC_LINK_H_ */
