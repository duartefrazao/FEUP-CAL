#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TravelAgency.h"
#include "Graph.h"



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
			break;
		case 2:
			nodeFilename = "nos2.txt";
			edgeFilename = "arestas2.txt";
			break;
		case 3:
			nodeFilename = "nos3.txt";
			edgeFilename = "arestas3.txt";
			break;
		case 4:
			nodeFilename = "nos4.txt";
			edgeFilename = "arestas4.txt";
			break;
		default:
			break;
		}
}

void TravelAgency::processGraph() {

	if(nodeFilename == "" || edgeFilename == ""){
		exit(12);
	}


	graph = new Graph<Location, Link>();



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

		int x = (X*750000 - 41.14022*750000);
		int y = (Y*92965.6 - -8.616468*92965.6);

//		std::cerr <<"Id: " << idNo << "\t" << "X: " << x << "\t" << "Y: " << y << std::endl;

		std::cerr << idNo << "\t" << X << "\t"  << Y << "\n";

		idNodes.insert({idNo, ++id});
		Location * location = new Location(id, x, y, x, y, "");
		locations.insert({id, location});
		graph->addVertex(location);
	}

	inFile.close();


	/*--------Edges--------*/
	inFile.open(edgeFilename);

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	int idAresta = 0;
	id  = 0;
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

		std::cerr << idAresta << "\t" << idNoOrigem << "\t" << idNoDestino << std::endl;
		Location * locationOrigem = locations.at(idNodes.at(idNoOrigem));
		Location * locationDestino = locations.at(idNodes.at(idNoDestino));
		graph->addEdge(locationOrigem, locationDestino, locationOrigem->distance(locationDestino), id++);
	}

	inFile.close();
}

void TravelAgency::createGraphViewer() {

	graphView = new GraphViewer(600, 600, false);
	graphView->createWindow(600, 600);
	graphView->defineEdgeColor("blue");
	graphView->defineVertexColor("yellow");
	graphView->defineEdgeCurved(false);


	vector<Location *> locations = graph->getVertexSet();

	for(Location * pLocation: locations){
		graphView->addNode(pLocation->getId(), pLocation->getX(), pLocation->getY());
		graphView->setVertexSize(pLocation->getId(), 5);
		for(Link link : pLocation->getAdj()){
			graphView->addEdge(link.getId(), pLocation->getId(), link.getDest()->getId(), EdgeType::DIRECTED);
		}

	}

}

void TravelAgency::visualizeGraph() {
	graphView->rearrange();
}


void TravelAgency::user() {



}


void TravelAgency::shortestPath() {

	int startNodeI, endNodeI;
	std::cout << "\nStart node index: ";
	std::cin >> startNodeI;

	std::cout << "End node index: ";
	std::cin >> endNodeI;

	Location * origin = locations.at(startNodeI);
	Location * dest = locations.at(endNodeI);


	vector<Location *> path;

	int algorithm;
	std::cout << "\nAlgorithm: "<<std::endl;
	std::cout << "1 - Dijkstra"<<std::endl;
	std::cout << "2 - A*"<<std::endl;
	std::cout << "Select: ";
	std::cin >> algorithm;

	switch(algorithm){
	case 1: path = graph->dijkstra(origin, dest); break;
	case 2: path = graph->aStar(origin, dest); break;
	}

	while(dest->path != NULL){
			int oldId = dest->getId();
			dest = static_cast<Location *> (dest->path);
			graphView->setVertexColor(dest->getId(), "green");

			for(Link link : dest->getAdj()){
				if(link.getDest()->getId() == oldId){
					graphView->setEdgeColor(link.getId(), "green");
					break;
				}
			}

		}
	visualizeGraph();
}


void TravelAgency::tsp() {

}
