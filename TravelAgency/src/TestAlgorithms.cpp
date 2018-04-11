#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <sstream>
#include <random>
#include <time.h>
#include <chrono>
#include "Graph.h"
#include "graphviewer.h"
#include <fstream>
#include <unordered_map>


using namespace std;

std::unordered_map<int, Location *> *locations = new std::unordered_map<int, Location *>();
GraphViewer *graphView;

void geneateRandomGridGraph(int n, Graph<Location, Link> & g) {
//    std::random_device rd;
//    std::mt19937 gen(rd());

	locations->clear();

	int idVertex = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++){
			Location * location = new Location(idVertex, (int) i*600/n, (int) j*600/n, "");
			locations->insert(std::make_pair(idVertex, location));
			g.addVertex(location);
			idVertex++;
		}

	int idEdge = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int di = -1; di <= 1; di++)
				for (int dj = -1; dj <= 1; dj++)
					if (i+di >= 0 && i+di < n && j+dj >= 0 && j+dj < n){
						Location * start = locations->at(i*n + j);
						Location * dest = locations->at((i+di) * n + (j+dj));
						g.addEdge(start, dest , start->distance(dest), idEdge++, "");
					}

	delete graphView;
	graphView = new GraphViewer(600, 600, false);
	graphView->createWindow(600, 600);
	graphView->defineEdgeColor("blue");
	graphView->defineVertexColor("yellow");
	vector<Location *> locationsG = g.getVertexSet();

	for(Location * pLocation: locationsG){
			graphView->addNode(pLocation->getId(), pLocation->getX(), pLocation->getY());

			for(Link link : pLocation->getAdj()){
				graphView->addEdge(link.getId(), pLocation->getId(), link.getDest()->getId(), EdgeType::DIRECTED);
			}

		}

	graphView->rearrange();
}

void test_performance_dijkstra() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 200; n += 10) {
		Graph<Location, Link> g;
		//cout << "Dijkstra generating grid " << n << " x " << n << " ..." << endl;
		geneateRandomGridGraph(n, g);
		//cout << "Dijkstra processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				Location * dest = locations->at(n*n-1);
				Location * ori = locations->at(i*n+j);
				g.dijkstra(ori, dest);
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
				graphView->rearrange();
				getchar();
			}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(finish - start).count();
		//std::cout << "Dijkstra processing grid " << n << " x " << n << " average time (micro-seconds)=" << (elapsed / (n*n)) << endl;
		std::cout << n*n << "\t" << (elapsed / (n*n) )<<std::endl;
	}
	std::cout << "--------------------------" << endl;
}


bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	s.push_back(CUTE(test_performance_dijkstra));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

//int main(int argc, char const *argv[]) {
//    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
//}

