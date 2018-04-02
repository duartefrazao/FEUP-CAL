

#ifndef SRC_SRC_LOCATION_H_
#define SRC_SRC_LOCATION_H_

#include <string>
#include <cmath>
#include "Link.h"
#include "Vertex.h"

class Location : public Vertex<Link>{
	int id;
	double x;
	double y;
	std::string name;

public:
	Location(int id, double x, double y, std::string name);
	bool operator==(const Location &l);
	double distance(Location *l);
	virtual ~Location();
	void addEdge(Location *d, double w);
	int getId() const;
};

#endif /* SRC_SRC_LOCATION_H_ */
