#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Graph.h"
#include "Location.h"
#include <unordered_map>
#include "Link.h"

void sleepU(int sec){

#ifdef __linux__
	sleep(sec);
#else
	Sleep(sec);
#endif
}


void test(std::string nodes, std::string edges)
{

	std::unordered_map<int, Location *> locations;
	Graph<Location, Link> *myGraph = new Graph<Location, Link>();


	GraphViewer *gv = new GraphViewer(600, 600, false);

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open(nodes);

	if (!inFile) {
	    cerr << "Unable to open file datafile.txt";
	    exit(1);   // call system to stop
	}

	std::string   line;

	int idNo=0;
	int X=0;
	int Y=0;

	while(std::getline(inFile, line))
	{
	    std::stringstream linestream(line);
	    std::string			data;

	    linestream >> idNo;

	    std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
	    linestream >> X;
	    std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
	    linestream >> Y;
	    gv->addNode(idNo,X,Y);


	    Location * location = new Location(idNo,X,Y, "");
	    locations.insert({idNo, location});
	    myGraph->addVertex(location);
	}

	inFile.close();


	//Ler o ficheiro arestas.txt
	inFile.open(edges);

		if (!inFile) {
		    cerr << "Unable to open file datafile.txt";
		    exit(1);   // call system to stop
		}

		int idAresta=0;
		int idNoOrigem=0;
		int idNoDestino=0;

		while(std::getline(inFile, line))
		{
		    std::stringstream linestream(line);
		    std::string data;


		    linestream >> idAresta;

		    std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		    linestream >> idNoOrigem;
		    std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		    linestream >> idNoDestino;
		    gv->addEdge(idAresta,idNoOrigem,idNoDestino, EdgeType::DIRECTED);

		    Location * locationOrigem = locations.at(idNoOrigem);
		    Location * locationDestino = locations.at(idNoDestino);
		    myGraph->addEdge(locationOrigem, locationDestino, locationOrigem->distance(locationDestino));
		}

		inFile.close();


		gv->rearrange();


		int mode;

		cout << "\n1 - One node to another\n";
		cout << "2 - Travel starting at one node and passing by a set of nodes\n";
		cout << "Type of operation: ";
		std::cin >> mode;

		if (mode == 1){

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
		case 1: path = myGraph->dijkstra(origin, dest); break;
		case 2: path = myGraph->aStar(origin, dest); break;
		}

		for(auto l : path){
			gv->setVertexColor(l->getId(), "green");
		}
		gv->rearrange();

		}
		else if (mode ==2){

		}


}

void inputTest(std::string &nodes, std::string &edges){
	int input;
	std::cout << "1 - Graph 1"<< std::endl;
	std::cout << "2 - Graph 2" << std::endl;
	std::cout << "Select graph: ";
	std::cin >> input;

	switch(input){
	case 1:
		nodes = "nos.txt";
		edges = "arestas.txt";
		break;
	case 2:
		nodes = "nos2.txt";
		edges = "arestas2.txt";
		break;
	default :break;
	}
}

int main() {
	std::string nodes;
	std::string edges;

	inputTest(nodes,edges);
	test(nodes, edges);
	getchar();
	return 0;
}
