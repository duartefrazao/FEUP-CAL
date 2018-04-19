#include "Vertex.h"
#include "Edge.h"
#include "MutablePriorityQueue.h"
#include <vector>
#include <limits.h>
#include <cfloat>
#include "Location.h"
#include <map>
#include <algorithm>

#define INF std::numeric_limits<double>::max()

template<class T>
double heuristic(Vertex<T> * v1, Vertex<T> *v2) {
	Location *t1 = static_cast<Location*>(v1);
	Location *t2 = static_cast<Location*>(v2);
	return t1->distance(t2);
}

template<class V, class E>
class Graph {

public:
	typedef double (*Heuristic)(Vertex<E>* v1, Vertex<E> *v2);

private:
	vector<V *> vertexSet;
	vector<vector<double>> weights;
	vector<vector<int>> next;
	vector<map<vector<V*>, pair<double, vector<V*>>>> hkAuxMemory;
	V* hkBegin;

protected:
	pair<double, vector<V*>> heldKarpAlgorithm(V* dest, vector<V*> places);
	bool aStarAlgorithm(V *origin, V *dest);
	void dijkstraShortestPath(V *s);

public:
	/*Graph manipulation functions*/
	Graph<V, E>();
	V *findVertex(const V &in) const;
	bool inVertexSet(V *in) const;
	bool addVertex(V *in);
	bool addEdge(V *sourc, V *dest, double w, unsigned long int id,
			std::string string);
	int getNumVertex() const;
	const vector<V *> &getVertexSet() const;
//	Heuristic heuristic;

	/*Shortest path algorithms*/
	vector<V *> dijkstra(V *origin, V *dest);
	vector<V *> aStar(V *origin, V *dest);
	void floydWarshallShortestPath();
	vector<int> getfloydWarshallPath(const V &origin, const V &dest) const;

	/*TSP algorithms*/
	vector<V*> heldKarpAlgorithm(vector<V*> dest);

};

template<class V, class E>
Graph<V, E>::Graph() {
	V::counter = 0;
	hkBegin = NULL;
}

template<class V, class E>
int Graph<V, E>::getNumVertex() const {
	return vertexSet.size();
}

template<class V, class E>
const vector<V *> &Graph<V, E>::getVertexSet() const {
	return vertexSet;
}

template<class V, class E>
bool Graph<V, E>::inVertexSet(V *in) const {
	for (auto v : vertexSet)
		if (v == in)
			return true;

	return false;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class V, class E>
bool Graph<V, E>::addVertex(V *in) {
	if (inVertexSet(in))
		return false;
	vertexSet.push_back(in);
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class V, class E>
bool Graph<V, E>::addEdge(V *sourc, V *dest, double w, unsigned long int id,
		std::string string) {
	if (!inVertexSet(sourc) || !inVertexSet(dest))
		return false;

	sourc->addEdge(dest, w, id, string);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

template<class V, class E>
void Graph<V, E>::dijkstraShortestPath(V *origin) {

	for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it) {
		(*it)->dist = INF;
		(*it)->inClosedSet = false;
		(*it)->path = NULL;
	}

	V * s = origin;

	if(s == NULL) return;

	MutablePriorityQueue<V> q;

	s->dist = 0;
	q.insert(s);

	V * v;

	while(!q.empty()) {

		v = q.extractMin();

		for(auto it = v->adj.begin(); it != v->adj.end(); ++it) {

			double newDist = v->dist + it->weight;

			if(newDist < it->dest->dist) {
				it->dest->dist = newDist;
				it->dest->path = v;

				if(it->dest->inClosedSet) {
					q.decreaseKey(it->dest);
				}
				else {
					q.insert(it->dest);
					it->dest->inClosedSet = true;
				}

			}
		}

	}

}

/**************** A STAR ************/
template<class V, class E>
bool Graph<V, E>::aStarAlgorithm(V *origin, V *dest) {

	MutablePriorityQueue<V> openSet;

	for (auto vertex : vertexSet) {
		vertex->f = INF;
		vertex->dist = INF;
		vertex->path = NULL;
		vertex->inClosedSet = false;
		vertex->inOpenSet = false;
	}

	V * end = dest;
	V * start = origin;

	start->dist = 0;
	start->updateF(heuristic, end);
	start->inClosedSet = false;
	start->inOpenSet = true;
	openSet.insert(start);

	while (!openSet.empty()) {

		V * v = openSet.extractMin();
		v->inClosedSet = true;
		v->inOpenSet = false;
		if (v == end) {
			return true;
		}

		for (E edge : v->adj) {
			V * destV = edge.getDest();
			if (destV->inClosedSet)
				continue;

			if (!destV->inOpenSet) {
				destV->dist = v->dist + edge.weight;
				destV->path = v;
				destV->updateF(heuristic, end);
				destV->inOpenSet = true;
				openSet.insert(destV);
				continue;
			}

			if (destV->calculateF(heuristic, end) < destV->f) {
				destV->dist = v->dist + edge.weight;
				destV->path = v;
				destV->updateF(heuristic, end);
			}

		}

	}

	return false;
}

template<class V, class E>
vector<V *> Graph<V, E>::aStar(V *origin, V *dest) {

	this->aStarAlgorithm(origin, dest);
	vector<V *> res;
	V * v = dest;

	while (v->path != NULL) {
		res.push_back(v);
		v = static_cast<V *>(v->path);
	}

	res.push_back(v);
	std::reverse(res.begin(), res.end());

	return res;
}

template<class V, class E>
vector<V *> Graph<V, E>::dijkstra(V *origin, V *dest) {

	this->dijkstraShortestPath(origin);
	vector<V *> res;
	V * v = dest;
	double distance = 0;

	while (v->path != NULL) {
		res.push_back(v);
		v = static_cast<V *>(v->path);
	}



	res.push_back(v);
	std::reverse(res.begin(), res.end());

	return res;
}
/**************** All Pairs Shortest Path  ***************/

template<class V, class E>
void Graph<V, E>::floydWarshallShortestPath() {

	vector<double> line(vertexSet.size(), INF);
	vector<vector<double>> newV(vertexSet.size(), line);

	vector<int> lineV(vertexSet.size(), INT_MAX);
	vector<vector<int>> newNext(vertexSet.size(), lineV);

	//Initialize array
	for (unsigned int i = 0; i < vertexSet.size(); i++) {
		for (unsigned int j = 0; j < vertexSet.size(); j++) {
			j == i ? newV.at(i).at(j) = 0 : newV.at(i).at(j) = INF;
		}
	}

	//Add weights from edges
	for (V* vertex : this->vertexSet) {
		for (E edge : vertex->adj) {
			newV.at(vertex->index).at(edge.dest->index) = edge.weight;
			newNext.at(vertex->index).at(edge.dest->index) = edge.dest->index;
		}
	}

	//Algorithm
	for (unsigned int k = 0; k < vertexSet.size(); k++) {
		for (unsigned int i = 0; i < vertexSet.size(); i++) {
			for (unsigned int j = 0; j < vertexSet.size(); j++) {
				if (newV.at(i).at(j)
						> newV.at(i).at(k)
						+ newV.at(k).at(
								j) && newV.at(i).at(k) != INF
						&& newV.at(k).at(j) != INF) {
					newV.at(i).at(j) = newV.at(i).at(k) + newV.at(k).at(j);
					newNext.at(i).at(j) = newNext.at(i).at(k);
				}
			}
		}
	}

	weights = newV;
	next = newNext;

}

template<class V, class E>
vector<int> Graph<V, E>::getfloydWarshallPath(const V &orig,
		const V &dest) const {

	int oIndex = orig.index;
	int dIndex = dest.index;
	vector<int> path;

	if (next.at(oIndex).at(dIndex) == INT_MAX)
		return path;

	path.push_back(oIndex);

	while (oIndex != dIndex) {
		oIndex = next.at(oIndex).at(dIndex);
		path.push_back(oIndex);
	}
	return path;

}

/*****************TRAVELING SALESMAN PROBLEM ALGORITHMS**********************/

template<class V, class E>
vector<V*> Graph<V, E>::heldKarpAlgorithm(vector<V*> dest) {

	/* Initializing the auxiliar memory structure */

	this->hkAuxMemory.clear();
	this->hkAuxMemory = vector<map<vector<V*>, pair<double, vector<V*>>>>(
			dest.size());
	this->floydWarshallShortestPath();
	hkBegin = dest.at(dest.size() - 1);

	dest.at(0)->auxIndex = 0;

	for (unsigned int i = 1; i < dest.size(); i++) {
		hkAuxMemory[i][ { }].first = weights[dest.at(0)->index][i];
		hkAuxMemory[i][ {}].second = {dest.at(i), hkBegin};
		dest.at(i)->auxIndex = i;
	}

	vector<V*> copyDest = dest;
	copyDest.erase(copyDest.begin());
	copyDest.pop_back();

	pair<double, vector<V*>> p = this->heldKarpAlgorithm(dest.at(0), copyDest);
	vector<V*> result = p.second;
	vector<V*> fullPath;
	if (p.first == INF) return fullPath;
	int endResult = result.size() - 1;
	for (int i = 0; i < endResult; i++) {
		fullPath.push_back(result.at(i));
		vector<int> intermediate = this->getfloydWarshallPath(*result.at(i),
				*result.at(i + 1));
		if (intermediate.size() > 2) {
			int interEnd = intermediate.size() - 1;
			for (int j = 1; j < interEnd; j++) {
				int index = intermediate.at(j);
				fullPath.push_back(vertexSet.at(index));
			}
		}

	}
	fullPath.push_back(result.at(endResult));

	double wey = 0;

	for (int i = 0; i < fullPath.size() - 1; i++) {
		for (Link e : fullPath.at(i)->getAdj()) {
			if (e.getDest()->getId() == fullPath.at(i + 1)->getId()) {
				wey += e.getWeight();
			}
		}
	}

	cout << "The time taken to perform the trip will be: " << wey * 1300
			<< " minutes" << endl;

	return fullPath;

}

template<class V, class E>
pair<double, vector<V*>> Graph<V, E>::heldKarpAlgorithm(V* dest,
		vector<V*> places) {
	pair<double, vector<V*>> p;

	try {
		p = hkAuxMemory[dest->auxIndex].at(places);

	} catch (out_of_range &e) {
		if (places.empty()) {
			p.first = weights[hkBegin->index][dest->index];
			p.second.insert(p.second.begin(), hkBegin);
			p.second.insert(p.second.begin(), dest);

		} else {
			pair<double, vector<V*>> auxResult;
			vector<V*> bestPath;
			double bestWeight = INF;

			for (unsigned int i = 0; i < places.size(); i++) {

				vector<V*> nextPlaces = places;
				V* nextDest = nextPlaces.at(i);
				nextPlaces.erase(nextPlaces.begin() + i);
				auxResult = heldKarpAlgorithm(nextDest, nextPlaces);
				double tempWeight = weights[dest->index][nextDest->index]
						+ auxResult.first;

				if (tempWeight < bestWeight) {
					bestPath = auxResult.second;
					bestWeight = tempWeight;
				}

			}

			vector<V*> finalPath = bestPath;
			finalPath.insert(finalPath.begin(), dest);

			//initializing p
			p.first = bestWeight;
			p.second = finalPath;

			//optimization
			hkAuxMemory[dest->auxIndex][places] = p;

		}
	}

//the wanted pair is already in the data structure
	return p;
}
