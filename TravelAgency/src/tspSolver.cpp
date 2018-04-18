#include "tspSolver.h"
#include "Graph.h"
#include <queue>
#include <unordered_set>
#include <utility>
#include <iostream>
#include "Vertex.h"
using namespace std;

tspSolver::tspSolver(Graph<Location, Link> * g, Location* s, Location* d,
		vector<Location *> p) :
		graph(g), source(s), dest(d), poi(p) {
}

tspSolver::~tspSolver() {

}
bool tspSolver::solveTSPGreedy() {

	vector<Location * > final;


	if (nearestNeighbour(source, dest, poi) && path.size() == poi.size()) {

		formatPath();
		twoOpt();

		vector<Location * > temp;

		vector<Location * > savedPath =path;



		Location * s = source;

		for(int i = 0; i < savedPath.size() -1;i++){

				temp=this->graph->aStar(savedPath.at(i),savedPath.at(i+1));

				s=savedPath.at(i);
				if(i != savedPath.size()-2)temp.pop_back();

				final.insert(final.end(),temp.begin(),temp.end());

		}

		for(int i = final.size()-1; i>0;i--){
			final.at(i)->path= final.at(i-1);
		}
		final.at(0)->path=NULL;

		path=final;

		finalPath=final;
		//std::reverse(final.begin(), final.end());

	}
}

bool tspSolver::solveTSPexperiments() {

	duartegoritmo();
	for (int i = 0; i < path.size(); i++) {
		cout << path.at(i)->getId() << "-> ";
	}

}
bool tspSolver::nearestNeighbour(Location* s, Location* d,
		vector<Location *> p) {

	if (p.size() == 0) {
		return true;
	}

	MutablePriorityQueue<Location> neighbours;

	for (Location* l : p) {
		l->setF(heuristic(s));
		neighbours.insert(l);
	}

	Location* next;

	while (!neighbours.empty()) {
		next = neighbours.extractMin();
		p = removeLocation(p, next);
		if (isPossible(next, p, s)&& nearestNeighbour(next, d, p)) {
			path.push_back(next);
			return true;
		}
	}
	return false;
}

vector<Location*> tspSolver::optSwap(int i, int k) {
	vector<Location*> newPath;
	vector<Location*> emptyPath;

	if (i - 2 >= 0 && !haveConnection(path.at(i - 2), path.at(k - 1)))
		return emptyPath;
	else if (!haveConnection(path.at(i - 1), path.at(path.size() - 1)))
		return emptyPath;

	for (int j = 0; j < i - 1; j++)
		newPath.push_back(path.at(j));

	for (int j = k - 1; j >= i - 1; j--) {
		if (j > i - 1 && !haveConnection(path.at(j), path.at(j - 1)))
			return emptyPath;
		newPath.push_back(path.at(j));
	}

	for (int j = k; j < path.size(); j++)
		newPath.push_back(path.at(j));

	return newPath;

}

double tspSolver::calculateTotalDistance(vector<Location*> route) {
	double dist = 0;
	vector<Location*> tempPath;
	for (int i = 0; i < route.size() - 1; i++) {
		tempPath = graph->aStar(route.at(i), route.at(i + 1));
		for (int j = 0; j < tempPath.size() - 1; j++) {
			dist += heuristic2(tempPath.at(j), tempPath.at(j + 1));
		}
	}
	return dist;
}

void tspSolver::twoOpt() {
	double bestDistance, newDistance;

	vector<Location*> newPath;
	bestDistance = calculateTotalDistance(path);

	while (true) {
		start_again: for (int i = 1; i < path.size() - 3; i++) {
			for (int k = i + 1; k < path.size() - 1; k++) {
				newPath = optSwap(i, k);
				if (newPath.size() > 0) {
					newDistance = calculateTotalDistance(newPath);
					if (newDistance < bestDistance) {
						path = newPath;
						bestDistance = newDistance;
						goto start_again;
					}
				}
			}
		}
		break;
	}

}

vector<Location*> tspSolver::removeLocation(vector<Location*> p,
		Location* next) {
	for (uint i = 0; i < p.size(); i++) {
		if (p.at(i)->getId() == next->getId()) {
			p.erase(p.begin() + i);
			break;
		}
	}
	return p;
}

bool tspSolver::bfs(Location* s, Location* d) {

	int idDest = d->getId();

	if (s->getId() == idDest)
		return true;

	queue<Location *> q;
	unordered_set<int> visited;

	q.push(s);

	pair<std::unordered_set<int>::iterator, bool> result;

	while (!q.empty()) {
		Location * l = q.front();
		q.pop();

		for (uint i = 0; i < l->getAdj().size(); i++) {

			if (l->getAdj().at(i).getDest()->getId() == idDest) {
				return true;
			}

			result = visited.insert(l->getAdj().at(i).getDest()->getId());
			if (result.second == true) {
				q.push(l->getAdj().at(i).getDest());
			}
		}
	}

	return false;

}

bool tspSolver::isPossible(Location* next, vector<Location*> p,Location* source) {
	if (!bfs(next, dest) || !bfs(source,next))
		return false;

	for (uint i = 0; i < p.size(); i++) {
		if (!bfs(next, p.at(i)))
			return false;
	}

	return true;
}





bool tspSolver::haveConnection(Location* a, Location* b) {
	if (!bfs(a, b))
		return false;

	return true;
}

void tspSolver::formatPath() {

	vector<Location*> final;

	final.push_back(source);
	for (int i = path.size() - 1; i >= 0; i--) {
		final.push_back(path.at(i));
	}
	final.push_back(dest);
	path = final;
}

vector<Location*> tspSolver::getPath() {
	return path;
}

double tspSolver::heuristic(Location* l2) {
	return source->distance(l2);
}
double tspSolver::heuristic2(Location* l1, Location* l2) {
	return l1->distance(l2);
}

void tspSolver::duartegoritmo() {
//	int x0, x1, x2, x3, y0, y1, y2, y3;
//
//	x1 = source->getX();
//	y1 = source->getY();
//
//	x2 = dest->getX();
//	y2 = dest->getY();
//
//	x0 = INT_MAX;
//	y0 = INT_MAX;
//
//	x3 = 0;
//	y3 = 0;
//
//	for (Location* l : poi) {
//		if (l->getX() < x0)
//			x0 = l->getX();
//		else if (l->getX() > x3)
//			x3 = l->getX();
//
//		if (l->getY() < y0)
//			y0 = l->getY();
//		else if (l->getY() > y3)
//			y3 = l->getY();
//	}
//
//	if (x0 == INT_MAX)
//		x0 = source->getX();
//	if (y0 == INT_MAX)
//		y0 = source->getY();
//
//	if (x3 == 0 || x3 < x2)
//		x3 = x2;
//	if (y3 == 0 || y3 < y2)
//		y3 = y2;
//
//	vector<vector<int>> secs;
//	int s = 10;
//	while (s-- > 0) {
//		vector<int> newV;
//		secs.push_back(newV);
//	}
//
//	for (int i = 0; i < poi.size(); i++) {
//		if (poi.at(i)->getX() <= x1 && poi.at(i)->getY() <= y1)
//			secs.at(7).push_back(i);
//		else if (poi.at(i)->getX() <= x2 && poi.at(i)->getY() <= y1)
//			secs.at(8).push_back(i);
//		else if (poi.at(i)->getX() <= x3 && poi.at(i)->getY() <= y1)
//			secs.at(9).push_back(i);
//		else if (poi.at(i)->getX() <= x1 && poi.at(i)->getY() <= y2)
//			secs.at(6).push_back(i);
//		else if (poi.at(i)->getX() <= x2 && poi.at(i)->getY() <= y2)
//			secs.at(5).push_back(i);
//		else if (poi.at(i)->getX() <= x3 && poi.at(i)->getY() <= y2)
//			secs.at(4).push_back(i);
//		else if (poi.at(i)->getX() <= x1 && poi.at(i)->getY() <= y3)
//			secs.at(1).push_back(i);
//		else if (poi.at(i)->getX() <= x2 && poi.at(i)->getY() <= y3)
//			secs.at(2).push_back(i);
//		else
//			secs.at(3).push_back(i);
//	}
//
//	Location* first = source;
//	MutablePriorityQueue<Location> pq;
//
//	vector<Location *> order;
//	order.push_back(first);
//
//	Location* temp = new Location(-1, x0, y3, "");
//
//	for (int i = 0; i <10; i++) {
//		for (int inS : secs.at(i)) {
//			poi.at(inS)->setF(heuristic2(poi.at(inS), temp));
//			pq.insert(poi.at(inS));
//		}
//
//		while (!pq.empty()) {
//			first = pq.extractMin();
//			order.push_back(first);
//		}
//
//	}
//
//	order.push_back(dest);
//
//	path=order;

}

const vector<Location*>& tspSolver::getFinalPath() const {
	return finalPath;
}

void tspSolver::setFinalPath(const vector<Location*>& finalPath) {
	this->finalPath = finalPath;
}
