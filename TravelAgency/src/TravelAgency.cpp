#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TravelAgency.h"
#include "Graph.h"

#define GV_WIDTH 600
#define GV_HEIGHT 600
#define GV_VERTEX_SIZE 7
#define GV_EDGE_THICKNESS 5

TravelAgency::TravelAgency() {
}

TravelAgency::~TravelAgency() {
}

void TravelAgency::chooseGraph() {
		int input;
		std::cout << "1 - Graph 1" << std::endl;
		std::cout << "2 - Graph 2" << std::endl;
		std::cout << "3 - Real Map" << std::endl;
		std::cout << "4 - Real Map 2" << std::endl;
		std::cout << "Select graph: ";
		std::cin >> input;

		switch (input) {
		case 1:
			nodeFilename = "nos.txt";
			edgeFilename = "arestas.txt";
			realMap = false;
			break;
		case 2:
			nodeFilename = "nos2.txt";
			edgeFilename = "arestas2.txt";
			realMap = false;
			break;
		case 3:
			nodeFilename = "nos3.txt";
			edgeFilename = "arestas3.txt";
			realMap = true;
			break;
		case 4:
			nodeFilename = "nos4.txt";
			edgeFilename = "arestas4.txt";
			namesFilename = "names4.txt";
			realMap = true;
			break;
		default:
			break;
		}
}

void TravelAgency::processGraph() {

	if(nodeFilename == "" || edgeFilename == ""){
		exit(1);
	}

	graph = new Graph<Location, Link>();
//	graph->heuristic = (Graph<Location, Link>::Heuristic) &TravelAgency::distanceHeuristic;

	ifstream inFile;


	/*--------Nodes--------*/
	inFile.open(nodeFilename);

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
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

		if(realMap){
			if(minLon == 0 || X < minLon){
				minLon = X;
			}
			else if(maxLon == 0 || X > maxLon){
				maxLon = X;
			}

			if(minLat == 0 || Y < minLat){
				minLat = Y;
			}
			else if(maxLat == 0 || Y > maxLat){
				maxLat = Y;
			}

			idNodes.insert({idNo, id});
			location = new Location(id, Y, X, "");
		}
		else{
			idNodes.insert({idNo, id});
			location = new Location(id, (int) X, (int) Y, "");
		}

		locations.insert({id, location});
		graph->addVertex(location);
	}

	inFile.close();

	int idAresta = 0;
	id  = 0;

	/*--------Names--------*/
	inFile.open(namesFilename);

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
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
		edgeNames.insert({idAresta, data});
	}

	inFile.close();



	/*--------Edges--------*/
	inFile.open(edgeFilename);

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
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

		if (it != edgeNames.end())
		{
			graph->addEdge(locationOrigem, locationDestino, locationOrigem->distance(locationDestino), id, it->second);
		}
		else
		{
			graph->addEdge(locationOrigem, locationDestino, locationOrigem->distance(locationDestino), id, "");
		}

//		id++;
//		graph->addEdge(locationDestino, locationOrigem , locationOrigem->distance(locationDestino), id);
	}

	inFile.close();



}


void TravelAgency::createGraphViewer() {
	graphView = new GraphViewer(GV_WIDTH, GV_HEIGHT, false);
	graphView->createWindow(GV_WIDTH, GV_HEIGHT);
	graphView->defineEdgeColor("blue");
	graphView->defineVertexColor("yellow");
	graphView->defineEdgeCurved(false);
	int id = 0;

	vector<Location *> vLocations = graph->getVertexSet();

	double coefX = GV_WIDTH/(maxLon - minLon);
	double coefY = GV_HEIGHT/(maxLat - minLat);


	for(Location * pLocation: vLocations){
		int x;
		int y;

		if(realMap){
			x = coefX * (pLocation->getLon() - minLon);
			y = coefY * (pLocation->getLat() - minLat);
		}
		else{
			x = pLocation->getX();
			y = pLocation->getY();
		}

		graphView->addNode(pLocation->getId(), x, y);
		graphView->setVertexSize(pLocation->getId(), GV_VERTEX_SIZE);
	}


	for(Location * pLocation: vLocations){
		for(Link link : pLocation->getAdj()){
			graphView->setVertexSize(link.getDest()->getId(), GV_VERTEX_SIZE);
			graphView->addEdge(link.getId(), pLocation->getId(), link.getDest()->getId(), EdgeType::DIRECTED);
			graphView->setEdgeLabel(link.getId(), link.getName());
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

	do{
		int option;

		std::cout << std::endl<<std::endl;
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

		switch(option){
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
	}while(exit == false);
}



void TravelAgency::travelMenu(){
	int day;
	int month;
	char c;
	std::string originStr;
	std::string destStr;
	char ans;

	std::cout << std::endl << std::endl;

	std::cout << "Travel " << std::endl;
	std::cout << "------" << std::endl;

	std::cout << std::endl;

	std::cout << "[?] Insert date (dd/mm): ";
	std::cin >> day >> c >> month;
	std::cin.ignore();
	std::cout << std::endl;

	std::cout << "[?] Travel from: ";
	std::getline(std::cin, originStr);
	origin = getLocation(originStr);

	if(origin == NULL){
		std::cerr << "[!] Node inexistent"<< std::endl;
		return;
	}

	std::cout << std::endl;

	std::cout << "[?] Travel to: ";
	std::getline(std::cin, destStr);
	destination = getLocation(destStr);

	if(destination == NULL){
		std::cerr << "[!] Node inexistent"<< std::endl;
		return;
	}

	std::cout << std::endl;

	std::cout << "[?] Do you want to specify places to visit (Y/N): ";
	std::cin >> ans;


	switch(toupper(ans)){
	case 'Y':{
		placesToVisit.push_back(origin);
		int locationID = 0;
		std::cout << "[?] Places (-1 to quit): "<<std::endl;
		while (true) {
			std::cout << " ->";
			cin >> locationID;
			if (locationID == -1)
				break;
			Location * location = locations.at(locationID);
			placesToVisit.push_back(location);
		}
		placesToVisit.push_back(destination);
		tsp();
		break;
	}
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
	std::cout << "\t----------"<< std::endl;

	std::cout << std::endl;

	std::cout << "\t[1] Dijkstra"<<std::endl;
	std::cout << "\t[2] A*"<<std::endl;

	std::cout << std::endl;

	std::cout << "[?] Select option: ";
	std::cin >> algorithm;

	std::cout << "Calculating..."<<std::endl;


	switch(algorithm){
	case 1:
		path = graph->dijkstra(origin, destination);
		break;
	case 2:
		path = graph->aStar(origin, destination);
		break;
	default: return;
	}

	if(destination->path == NULL){
		std::cout << "[!] Impossible path"<<std::endl;
	}
	else{
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

	vector<Location *> path;

	int algorithm;

	std::cout << std::endl;
	std::cout << std::endl;


	std::cout << "\tAlgorithm" << std::endl;
	std::cout << "\t----------"<< std::endl;

	std::cout << std::endl;

	std::cout << "\t[1] Held-Karp"<<std::endl;
	std::cout << "\t[2] Greedy(renomear)"<<std::endl;

	std::cout << std::endl;

	std::cout << "[?] Select option: ";
	std::cin >> algorithm;

	std::cout << "Calculating..."<<std::endl;


	switch(algorithm){
	case 1:
		path = graph->heldKarpAlgorithm(placesToVisit);
		break;
	case 2:
		//TODO: Acrescentar algoritmo Duarte
		//path = graph->aStar(origin, destination);
		break;
	default: return;
	}



	if(destination->path == NULL || path.size() == 1){
		std::cout << "[!] Impossible path"<<std::endl;
	}
	else{
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



void TravelAgency::closeGraphView() {
	if(graphView != NULL){
		graphView->closeWindow();
		delete graphView;
		graphView = NULL;
	}
}

bool TravelAgency::drawPath(){

	graphView->setVertexColor(destination->getId(), "green");

	while(destination->path != NULL){
		int oldId = destination->getId();
		destination = static_cast<Location *> (destination->path);
		graphView->setVertexColor(destination->getId(), "green");

		for(Link link : destination->getAdj()){
			if(link.getDest()->getId() == oldId){
				graphView->setEdgeColor(link.getId(), "green");
				graphView->setEdgeThickness(link.getId(), GV_EDGE_THICKNESS);
				break;
			}
		}

	}

	return true;
}

double TravelAgency::distanceHeuristic(Location* l1, Location* l2) {
	std::cerr << "Called " << l1->distance(l2) << std::endl;
	return l1->distance(l2);
}

Location* TravelAgency::getLocation(std::string allocator) {

	for(auto pr : locations){
		Location * pLocation = pr.second;
		for(Link link : pLocation->getAdj()){
			if(link.getName() == allocator){
				return pLocation;
			}
		}
	}

	return NULL;

}

double TravelAgency::costHeuristic(Location* l1, Location* l2) {
	return 0.0;
}