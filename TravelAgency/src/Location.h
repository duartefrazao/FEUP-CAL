

#ifndef SRC_SRC_LOCATION_H_
#define SRC_SRC_LOCATION_H_

#include <string>
#include <cmath>
#include "Link.h"
#include "Vertex.h"




class Location : public Vertex<Link>{
	unsigned long int id;
	double x;
	double y;
	double lat;
	double lon;
	std::string name;

public:
	Location(unsigned long int id, int x, int y, double lat, double lon, std::string name);
	bool operator==(const Location &l);
	bool operator<(const Location &l);
	double distance(Location *l);
	virtual ~Location();
	void addEdge(Location *d, double w, unsigned long int id);
	int getId() const;
	double getX() const;
	double getY() const;
};


#endif /* SRC_SRC_LOCATION_H_ */
