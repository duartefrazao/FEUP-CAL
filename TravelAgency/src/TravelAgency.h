#ifndef SRC_TRAVELAGENCY_H_
#define SRC_TRAVELAGENCY_H_

#include "Location.h"
#include "graphviewer.h"
#include <unordered_map>

class TravelAgency {
private:
	std::vector<Location *> placesToVisit;
	Location * origin;
	Location * destination;
	std::string nodeFilename;
	std::string edgeFilename;
	Graph<Location, Link> *graph;
	GraphViewer *graphView;
	std::unordered_map<unsigned long int, Location *> locations;
	std::unordered_map<unsigned long int, int> idNodes;

public:
	TravelAgency();
	virtual ~TravelAgency();
	void chooseGraph();
	void processGraph();
	void createGraphViewer();
	void visualizeGraph();
	void user();
	void shortestPath();
	void tsp();

	double distanceHeuristic(Location * l1, Location *l2);
	double costHeuristic(Location * l1, Location *l2);

};

#endif /* SRC_TRAVELAGENCY_H_ */
