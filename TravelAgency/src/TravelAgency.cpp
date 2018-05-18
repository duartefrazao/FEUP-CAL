#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "TravelAgency.h"
#include "Graph.h"
#include "tspSolver.h"
#include "StringMatcher.h"
#include <set>

#define GV_WIDTH 600
#define GV_HEIGHT 600
#define GV_VERTEX_SIZE 7
#define GV_EDGE_THICKNESS 5

TravelAgency::TravelAgency() {
}

TravelAgency::~TravelAgency() {
}

void TravelAgency::chooseGraph() {
	nodeFilename = "maps/nodes3.txt";
	edgeFilename = "maps/routes3.txt";
	namesFilename = "maps/names3.txt";
	realMap = true;
}

void TravelAgency::processGraph() {

	if (nodeFilename == "" || edgeFilename == "") {
		exit(1);
	}

	graph = new Graph<Location, Link>();

	ifstream inFile;

	/*--------Nodes--------*/
	inFile.open(nodeFilename);

	if (!inFile) {
		cerr << "Unable to open file " << nodeFilename;
		exit(1);   // call system to stop
	}

	std::string line;

	unsigned long int idNo = 0;
	double X = 0;
	double Y = 0;
	int id = 0;

	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;

		linestream >> idNo;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> X;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> Y;
		++id;

		Location * location;

		if (realMap) {
			if (minLon == 0 || X < minLon) {
				minLon = X;
			} else if (maxLon == 0 || X > maxLon) {
				maxLon = X;
			}

			if (minLat == 0 || Y < minLat) {
				minLat = Y;
			} else if (maxLat == 0 || Y > maxLat) {
				maxLat = Y;
			}

			idNodes.insert( { idNo, id });
			location = new Location(id, Y, X, "");
		} else {
			idNodes.insert( { idNo, id });
			location = new Location(id, (int) X, (int) Y, "");
		}

		locations.insert( { id, location });
		graph->addVertex(location);
	}

	inFile.close();

	int idAresta = 0;
	id = 0;

	/*--------Names--------*/
	inFile.open(namesFilename);

	if (!inFile) {
		cerr << "Unable to open file " << namesFilename;
		exit(1);   // call system to stop
	}

	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;
		std::string name;

		linestream >> idAresta;
		std::getline(linestream, data, ';');
		std::getline(linestream, data, '\n');
		data.pop_back();
		edgeNames.insert( { idAresta, data });
	}

	inFile.close();

	/*--------Edges--------*/
	inFile.open(edgeFilename);

	if (!inFile) {
		cerr << "Unable to open file " << edgeFilename;
		exit(1);   // call system to stop
	}

	unsigned long int idNoOrigem = 0;
	unsigned long int idNoDestino = 0;

	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;

		linestream >> idAresta;

		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> idNoOrigem;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> idNoDestino;

		++id;
		Location * locationOrigem = locations.at(idNodes.at(idNoOrigem));
		Location * locationDestino = locations.at(idNodes.at(idNoDestino));

		auto it = edgeNames.find(idAresta);

		if (it != edgeNames.end()) {
			graph->addEdge(locationOrigem, locationDestino,
					locationOrigem->distance(locationDestino), id, it->second);
		} else {
			graph->addEdge(locationOrigem, locationDestino,
					locationOrigem->distance(locationDestino), id, "");
		}

		id++;
		graph->addEdge(locationDestino, locationOrigem,
				locationOrigem->distance(locationDestino), id, "");

	}

	inFile.close();

}

void TravelAgency::createGraphViewer() {
	delete (graphView);
	graphView = new GraphViewer(GV_WIDTH, GV_HEIGHT, false);
	graphView->createWindow(GV_WIDTH, GV_HEIGHT);
	graphView->defineEdgeColor("blue");
	graphView->defineVertexColor("yellow");
	graphView->defineEdgeCurved(false);

	vector<Location *> vLocations = graph->getVertexSet();

	double coefX = GV_WIDTH / (maxLon - minLon);
	double coefY = GV_HEIGHT / (maxLat - minLat);

	for (Location * pLocation : vLocations) {
		int x;
		int y;

		int temp;

		if (realMap) {
			x = coefX * (pLocation->getLon() - minLon);
			y = coefY * (pLocation->getLat() - minLat);

			temp = x;
			x = y;
			y = temp;

		} else {
			x = pLocation->getX();
			y = pLocation->getY();
		}

		graphView->addNode(pLocation->getId(), x, y);
		graphView->setVertexSize(pLocation->getId(), GV_VERTEX_SIZE);
	}

	for (Location * pLocation : vLocations) {
		for (Link link : pLocation->getAdj()) {
			graphView->setVertexSize(link.getDest()->getId(), GV_VERTEX_SIZE);
			graphView->addEdge(link.getId(), pLocation->getId(),
					link.getDest()->getId(), EdgeType::DIRECTED);
			//graphView->setEdgeLabel(link.getId(), link.getName());
		}
	}
}

void TravelAgency::visualizeGraph() {
	createGraphViewer();
	graphView->rearrange();
	std::cout << "Press ENTER to continue ...";
	std::cin.ignore(1000, '\n');
	getchar();
	closeGraphView();
}

void TravelAgency::mainMenu() {

	bool exit = false;

	do {
		int option;

		std::cout << std::endl << std::endl;
		std::cout << "\t---------------" << std::endl;
		std::cout << "\t|Travel Agency|" << std::endl;
		std::cout << "\t---------------" << std::endl;

		std::cout << std::endl;

		std::cout << "\t[1] Visualize map" << std::endl;
		std::cout << "\t[2] Travel" << std::endl;
		std::cout << "\t[3] Exit" << std::endl;

		std::cout << std::endl;

		std::cout << "[?] Select option: ";
		std::cin >> option;

		switch (option) {
		case 1:
			visualizeGraph();
			break;
		case 2:
			travelMenu();
			break;
		case 3:
			exit = true;
			break;
		default:
			break;
		}
	} while (exit == false);
}

void TravelAgency::travelMenu() {

	std::string originStr;
	std::string destStr;
	char ans;

	std::cout << std::endl << std::endl;

	std::cout << "Travel " << std::endl;
	std::cout << "------" << std::endl;

	std::cout << std::endl;

	std::cin.ignore(1000, '\n');

	std::cout << "[?] Travel from: ";
	std::getline(std::cin, originStr);

	origin = getLocation(originStr);



	if (origin == NULL) {
		std::cerr << "[!] Node inexistent" << std::endl;
		return;
	}

	std::cout << std::endl;
	std::cin.ignore(1000, '\n');
	std::cout << "[?] Travel to: ";
	std::getline(std::cin, destStr);
	destination = getLocation(destStr);

	if (destination == NULL) {
		std::cerr << "[!] Node inexistent" << std::endl;
		return;
	}

	std::cout << std::endl;

	std::cout << "[?] Do you want to specify places to visit (Y/N): ";
	std::cin >> ans;
	std::cin.ignore();

	switch (toupper(ans)) {
	case 'Y':
		tsp();
		break;
	case 'N':
		shortestPath();
		break;
	default:
		break;
	}
}

void TravelAgency::shortestPath() {
	vector<Location *> path;

	int algorithm;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "\tAlgorithm" << std::endl;
	std::cout << "\t----------" << std::endl;

	std::cout << std::endl;

	std::cout << "\t[1] Dijkstra" << std::endl;
	std::cout << "\t[2] A*" << std::endl;

	std::cout << std::endl;

	std::cout << "[?] Select option: ";
	std::cin >> algorithm;

	std::cout << "Calculating..." << std::endl;

	switch (algorithm) {
	case 1:
		path = graph->dijkstra(origin, destination);
		break;
	case 2:
		path = graph->aStar(origin, destination);
		break;
	default:
		return;
	}

	if (destination->path == NULL) {
		std::cout << "[!] Impossible path" << std::endl;
	} else {
		createGraphViewer();
		drawPath();
		std::cout << "[!] Finished" << std::endl;
		graphView->rearrange();
		std::cout << std::endl;
		std::cout << "Press ENTER to continue ...";
		std::cin.ignore(1000, '\n');
		getchar();
		closeGraphView();
	}
}

void TravelAgency::tsp() {

	std::string locationStr;
	std::cout << "[?] Places (ENTER to quit): " << std::endl;
	while (true) {
		std::cout << " ->";
		std::getline(std::cin, locationStr);
		if (locationStr == "")
			break;
		Location * location = getLocation(locationStr);

		if (location == NULL) {
			std::cerr << "[!] Node inexistent" << std::endl;
		} else {
			placesToVisit.push_back(location);
		}

	}

	if (placesToVisit.empty())
		shortestPath();

	vector<Location *> path;

	int algorithm;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "\tAlgorithm" << std::endl;
	std::cout << "\t----------" << std::endl;

	std::cout << std::endl;

	std::cout << "\t[1] Held-Karp" << std::endl;
	std::cout << "\t[2] Greedy" << std::endl;

	std::cout << std::endl;

	std::cout << "[?] Select option: ";
	std::cin >> algorithm;

	std::cout << "Calculating..." << std::endl;

	vector<Location*> hkPlaces = placesToVisit;
	hkPlaces.insert(hkPlaces.begin(), origin);
	hkPlaces.push_back(destination);

	tspSolver* tsp;
	bool impossible;

	switch (algorithm) {
	case 1:
		path = graph->heldKarpAlgorithm(hkPlaces);
		impossible = path.empty();
		break;
	case 2: {
		tsp = new tspSolver(graph, origin, destination, placesToVisit);
		tsp->solveTSPGreedy();
		path = tsp->getFinalPath();
		impossible = destination->path == NULL;
		break;
	}
	default:
		return;
	}

	if (impossible) {
		std::cout << "[!] Impossible path" << std::endl;
	} else {
		createGraphViewer();
		testDrawPath(path);
		std::cout << "[!] Finished" << std::endl;
		graphView->rearrange();
		std::cout << std::endl;
		std::cout << "Press ENTER to continue ...";
		std::cin.ignore(1000, '\n');
		getchar();
		closeGraphView();
	}
}

void TravelAgency::closeGraphView() {
	if (graphView != NULL) {
		graphView->closeWindow();
		delete graphView;
		graphView = NULL;
	}
}

bool TravelAgency::drawPath() {

	graphView->setVertexColor(destination->getId(), "green");

	double wey = 0;
	while (destination->path != NULL) {
		int oldId = destination->getId();
		destination = static_cast<Location *>(destination->path);
		graphView->setVertexColor(destination->getId(), "green");

		for (Link link : destination->getAdj()) {
			if (link.getDest()->getId() == oldId) {
				wey += link.getWeight();
				graphView->setEdgeColor(link.getId(), "green");
				graphView->setEdgeThickness(link.getId(), GV_EDGE_THICKNESS);
				break;
			}
		}

	}
	cout << "The time taken to perform the trip will be: " << wey * 1300
			<< " minutes" << endl;

	return true;
}

Location* TravelAgency::getLocation(std::string pattern) {

	string processedPattern  = preProcessingChars(pattern);

	for (auto pr : locations) {
		Location * pLocation = pr.second;
		for (Link link : pLocation->getAdj()) {
			if (kmpMatcher(preProcessingChars(link.getName()), preProcessingChars(pattern))) {
				cout << "Text: " << link.getName() << endl;
				cout << "Pattern: " << pattern << endl;
				return pLocation;
			}
		}
	}

	vector<pair<Link, Location *>> similarLocations;

	vector<string> vPattern = preProcessString(processedPattern);

	set<string> inSimilar;

	for (auto pr : locations) {
		Location * pLocation = pr.second;
		for (Link link : pLocation->getAdj()) {

			vector<string> vText = preProcessString(preProcessingChars(link.getName()));

			int distance = 0;
			int patternSize = 0;

			for(unsigned int i = 0; i < vPattern.size(); i++){
				int minDistance = vPattern.at(i).length();
				patternSize += minDistance;

				for(unsigned int j = 0; j < vText.size(); j++){
					int d = editDistance(vPattern.at(i), vText.at(j));

					if(d < minDistance)
						minDistance = d;

				}

				distance += minDistance;

			}


			if(1 - distance * 1.0 / patternSize > HIT_RATE){
				if(inSimilar.insert(link.getName()).second)similarLocations.push_back(make_pair(link, pLocation));
			}

		}
	}


	if(similarLocations.size() != 0){
		for(unsigned int i = 0; i < similarLocations.size(); i++){
				cout << "[" << i << "] - " << similarLocations.at(i).first.getName() << endl;
			}



			int option;

			std::cout << "[?] Select option: ";
			std::cin >> option;

			if(option >= 0 && option < similarLocations.size()){
				return similarLocations.at(option).second;
			}
	}


	return NULL;
}

double TravelAgency::distanceHeuristic(Location* l1, Location* l2) {
	std::cerr << "Called " << l1->distance(l2) << std::endl;
	return l1->distance(l2);
}

bool TravelAgency::testDrawPath(vector<Location*> v) {

	if (!v.empty())
		graphView->setVertexColor(v.at(0)->getId(), "green");

	vector<bool> t(graph->getVertexSet().size(), false);
	Location * temp;

	for (int i = 0; i < v.size(); i++) {
		int oldId = v.at(i)->getId();
		temp = static_cast<Location *>(v.at(i)->path);
		usleep(200000);
		if (t[v[i]->index]) {
			graphView->setVertexColor(v.at(i)->getId(), "blue");
			graphView->setVertexSize(v.at(i)->getId(), 10);
			t[v[i]->index] = false;
		} else {
			graphView->setVertexColor(v.at(i)->getId(), "green");
			graphView->setVertexSize(v.at(i)->getId(), 10);
			t[v[i]->index] = true;
		}
		graphView->rearrange();
		/*
		 for (Link link : v.at(i)->getAdj()) {
		 if (link.getDest()->getId() == oldId) {
		 usleep(200000);
		 if (t[v[i]->index]) {

		 cout << "else";
		 graphView->setVertexColor(v.at(i)->getId(), "blue");
		 graphView->setVertexSize(v.at(i)->getId(), 10);
		 t[v[i]->index] = false;
		 } else {
		 graphView->setVertexColor(v.at(i)->getId(), "green");
		 graphView->setVertexSize(v.at(i)->getId(), 10);
		 t[v[i]->index] = true;
		 }

		 graphView->rearrange();
		 graphView->setEdgeThickness(link.getId(), GV_EDGE_THICKNESS);
		 break;
		 }
		 }
		 */
	}

	return true;
}

double TravelAgency::costHeuristic(Location* l1, Location* l2) {
	return 0.0;
}
