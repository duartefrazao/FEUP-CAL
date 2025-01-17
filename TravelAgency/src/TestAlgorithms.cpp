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
#include "StringMatcher.h"
#include <fstream>
#include <unordered_map>
#include "tspSolver.h"

using namespace std;

std::unordered_map<int, Location *> *locations = new std::unordered_map<int,
		Location *>();
GraphViewer *graphView;

void geneateRandomGridGraph(int n, Graph<Location, Link> & g) {
//    std::random_device rd;
//    std::mt19937 gen(rd());

	locations->clear();

	int idVertex = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			Location * location = new Location(idVertex, (int) i * 600 / n,
					(int) j * 600 / n, "");
			locations->insert(std::make_pair(idVertex, location));
			g.addVertex(location);
			idVertex++;
		}

	int idEdge = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int di = -1; di <= 1; di++)
				for (int dj = -1; dj <= 1; dj++)
					if (i + di >= 0 && i + di < n && j + dj >= 0
							&& j + dj < n) {
						Location * start = locations->at(i * n + j);
						Location * dest = locations->at(
								(i + di) * n + (j + dj));
						g.addEdge(start, dest, start->distance(dest), idEdge++,
								"");
					}

	delete graphView;
	graphView = new GraphViewer(600, 600, false);
	graphView->createWindow(600, 600);
	graphView->defineEdgeColor("blue");
	graphView->defineVertexColor("yellow");
	vector<Location *> locationsG = g.getVertexSet();

	for (Location * pLocation : locationsG) {
		graphView->addNode(pLocation->getId(), pLocation->getX(),
				pLocation->getY());

		for (Link link : pLocation->getAdj()) {
			graphView->addEdge(link.getId(), pLocation->getId(),
					link.getDest()->getId(), EdgeType::DIRECTED);
		}

	}

	graphView->rearrange();
}

void test_performance_dijkstra() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 200; n += 10) {
		Graph<Location, Link> g;
		cout << "Dijkstra generating grid " << n << " x " << n << " ..."
				<< endl;
		geneateRandomGridGraph(n, g);
		//cout << "Dijkstra processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				Location * dest = locations->at(n * n - 1);
				Location * ori = locations->at(i * n + j);
				g.dijkstra(ori, dest);
				while (dest->path != NULL) {
					int oldId = dest->getId();
					dest = static_cast<Location *>(dest->path);
					graphView->setVertexColor(dest->getId(), "green");

					for (Link link : dest->getAdj()) {
						if (link.getDest()->getId() == oldId) {
							graphView->setEdgeColor(link.getId(), "green");
							break;
						}
					}

				}
				graphView->rearrange();
				getchar();
			}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		//std::cout << "Dijkstra processing grid " << n << " x " << n << " average time (micro-seconds)=" << (elapsed / (n*n)) << endl;
		std::cout << n * n << "\t" << (elapsed / (n * n)) << std::endl;
	}
	std::cout << "--------------------------" << endl;
}

void test_performance_warshall() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 20; n += 1) {
		Graph<Location, Link> gp;
		//cout << "TSP greedy generating grid " << n << " x " << n << " ..." << endl;
		geneateRandomGridGraph(n, gp);
		//cout << "TSP greedy processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();

		Graph<Location, Link> * g = &gp;

		for (int i = 0; i < n; i++) {

			Location * dest = locations->at(n - 1 - i);
			Location * ori = locations->at(i);
			g->floydWarshallShortestPath();
			g->getfloydWarshallPath(*ori, *dest);

		}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		std::cout << "TSP greedy processing grid " << n << " x " << n
				<< " average time (micro-seconds)=" << (elapsed / (n * n))
				<< endl;
		std::cout << n * n << "\t" << (elapsed / (n * n)) << std::endl;
	}
	std::cout << "--------------------------" << endl;
}

void test_performance_tspGreedy() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 20; n += 1) {
		Graph<Location, Link> gp;
		//cout << "TSP greedy generating grid " << n << " x " << n << " ..." << endl;
		geneateRandomGridGraph(n, gp);
		//cout << "TSP greedy processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();

		Graph<Location, Link> * g = &gp;

		for (int i = 0; i < n; i++) {

			Location * dest = locations->at(n - 1 - i);
			Location * ori = locations->at(i);

			vector<Location*> p;

			for (int k = 0; k < n; k += n / 10)
				p.push_back(g->getVertexSet().at(k));

			tspSolver * tsp = new tspSolver(g, ori, dest, p);

			tsp->solveTSPGreedy();

		}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		std::cout << "TSP greedy processing grid " << n << " x " << n
				<< " average time (micro-seconds)=" << (elapsed / (n * n))
				<< endl;
		std::cout << n * n << "\t" << (elapsed / (n * n)) << std::endl;
	}
	std::cout << "--------------------------" << endl;
}

void test_performance_tspGreedyBack() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 20; n += 1) {
		Graph<Location, Link> gp;
		//cout << "TSP greedy generating grid " << n << " x " << n << " ..." << endl;
		geneateRandomGridGraph(n, gp);
		//cout << "TSP greedy processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();

		Graph<Location, Link> * g = &gp;

		for (int i = 0; i < n; i++) {

			Location * dest = locations->at(i);
			Location * ori = locations->at(n - 1 - i);

			vector<Location*> p;

			for (int k = 0; k < n; k += n / 10)
				p.push_back(g->getVertexSet().at(k));
			p.push_back(dest);

			tspSolver * tsp = new tspSolver(g, ori, ori, p);

			tsp->solveTSPGreedy();

		}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		std::cout << "TSP greedy Back processing grid " << n << " x " << n
				<< " average time (micro-seconds)=" << (elapsed / (n * n))
				<< endl;
		std::cout << n * n << "\t" << (elapsed / (n * n)) << std::endl;
	}
	std::cout << "--------------------------" << endl;
}

void test_performance_tspHK() {
	ofstream status;
	ofstream grid;

	for (int n = 10; n <= 20; n += 1) {
		Graph<Location, Link> gp;
		//cout << "TSP HK generating grid " << n << " x " << n << " ..." << endl;
		geneateRandomGridGraph(n, gp);
		//cout << "TSP HK processing grid " << n << " x " << n << " ..." << endl;
		auto start = std::chrono::high_resolution_clock::now();

		Graph<Location, Link> * g = &gp;

		for (int i = 0; i < n; i++) {
			Location * dest = locations->at(i);
			Location * ori = locations->at(n - 1);

			vector<Location*> p;

			p.push_back(ori);

			for (int k = 0; k < n; k += n / 10)
				p.push_back(g->getVertexSet().at(k));

			p.push_back(dest);

			g->heldKarpAlgorithm(p);

		}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		std::cout << "TSP HK processing grid " << n << " x " << n
				<< " average time (micro-seconds)=" << (elapsed / (n * n))
				<< endl;
		std::cout << n * n << "\t" << (elapsed / (n * n)) << std::endl;
	}
	std::cout << "--------------------------" << endl;
}

string generateRandomString(int size) {
	char c;
	int r;

	string res;

	for (int i = 0; i < size; i++) {
		r = rand() % 26;
		c = 'a' + r;
		res += string(1, c);
	}
	return res;
}

string generateRandomRealString(int size) {
	const char alphanum[] =
			"aaaaaaaaaaaaaaabcdeeeeeeeeefghiiiiiiiiijklmnoooooopqrstuuuuvwxyz";
	string s = "";
	for (int i = 0; i < size; ++i) {
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return s;

}

void comparison_test() {

	//Patterns size
	for (int n = 50; n <= 1000; n += 50) {
		auto start = std::chrono::high_resolution_clock::now();
		//Texts size
		for (int i = 50; i <= 1000; i += 50) {
			//p,t
			//string pattern= generateRandomString(n);
			//string text= generateRandomString(n);
			editDistance(generateRandomString(n), generateRandomString(i));
			//cout << "Testing pattern: "<<pattern << " with text: "<< text<< " dist: "<< editDistance(pattern,text) << endl;
		}
		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		cout << "Time taken for editDistance algorithm, pattern size=" << n
				<< " average time (micro-seconds)=" << (elapsed / 51)
				<< std::endl;
	}

}

void comparison_test_simple() {

	//Patterns size
	for (int n = 50; n <= 1000; n += 50) {
		auto start = std::chrono::high_resolution_clock::now();
		//Texts size

		editDistance(generateRandomString(n), generateRandomString(n));

		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		cout << "Time taken for editDistance algorithm, size=" << n
				<< " average time (micro-seconds)=" << elapsed << std::endl;
	}
}

void exact_test() {
	vector<double> naive(20, 0);
	vector<double> kmp(20, 0);
	int i = 0;
	int nTests = 10000;

	//Patterns size

	for (int n = 50; n <= 1000; n += 50) {

		for (int k = 0; k < nTests; k++) {
			//Texts size
			std::string text = generateRandomRealString(n);
			std::string pattern = generateRandomRealString(n / 2);
			auto start = std::chrono::high_resolution_clock::now();
			kmpMatcher(text, pattern);

			auto finish = std::chrono::high_resolution_clock::now();
			auto elapsed = chrono::duration_cast<chrono::microseconds>(
					finish - start).count();
			/*
			 cout << "Time taken for kmp algorithm, size=" << n
			 << " average time (micro-seconds)=" << elapsed << std::endl;
			 */
			kmp[i] += elapsed;

			auto start2 = std::chrono::high_resolution_clock::now();
			naiveMatcher(text, pattern);

			auto finish2 = std::chrono::high_resolution_clock::now();
			auto elapsed2 = chrono::duration_cast<chrono::microseconds>(
					finish2 - start2).count();
			/*
			 cout << "Time taken for naivealgorithm, size=" << n
			 << " average time (micro-seconds)=" << elapsed2 << std::endl;
			 */
			naive[i] += elapsed2;

		}
		kmp[i] /= nTests;
		naive[i] /= nTests;
		i++;
	}

	for (int j = 0; j < 20; j++) {

		cout << "Time taken for kmp algorithm, size=" << j
				<< " average time (micro-seconds)=" << kmp[j] << std::endl;

		cout << "Time taken for naive algorithm, size=" << j
				<< " average time (micro-seconds)=" << naive[j] << std::endl;

	}
}

void naive_test() {

	//Patterns size
	for (int n = 50; n <= 1000; n += 50) {
		auto start = std::chrono::high_resolution_clock::now();
		//Texts size

		naiveMatcher(generateRandomString(n), generateRandomString(n));

		auto finish = std::chrono::high_resolution_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(
				finish - start).count();
		cout << "Time taken for naive algorithm, size=" << n
				<< " average time (micro-seconds)=" << elapsed << std::endl;
	}
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//s.push_back(CUTE(test_performance_dijkstra));
	//s.push_back(CUTE(test_performance_tspGreedy));
	//s.push_back(CUTE(test_performance_tspGreedyBack));
	//s.push_back(CUTE(test_performance_tspHK));
	//s.push_back(CUTE(test_performance_warshall));
	s.push_back(CUTE(exact_test));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
	return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}

