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
	std::string namesFilename;
	Graph<Location, Link> *graph;
	GraphViewer *graphView;

	std::unordered_map<int, Location *> locations;
	std::unordered_map<unsigned long int, int> idNodes;
	std::unordered_map<unsigned long int, std::string> edgeNames;
	bool realMap;

	double minLat = 0;
	double maxLat = 0;
	double minLon = 0;
	double maxLon = 0;

public:
	TravelAgency();
	virtual ~TravelAgency();
	void chooseGraph();
	void processGraph();
	void createGraphViewer();
	void visualizeGraph();
	void mainMenu();
	void closeGraphView();
	void travelMenu();
	void shortestPath();
	void tsp();
	bool drawPath();
	bool testDrawPath(vector<Location *> v);
	Location * getLocation(std::string);

	static double distanceHeuristic(Location * l1, Location *l2);
	static double costHeuristic(Location * l1, Location *l2);

};

#endif /* SRC_TRAVELAGENCY_H_ */
