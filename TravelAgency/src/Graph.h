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
	vector<V *> vertexSet;    // vertex set
	vector<vector<double>> weights;
	vector<vector<int>> next;
	vector<map<vector<V*>, pair<double, vector<V*>>>> hkAuxMemory;
	V* hkBegin;

public:

	Graph<V, E>();
	V *findVertex(const V &in) const;
	bool inVertexSet(V *in) const;
	bool addVertex(V *in);
	bool addEdge(V *sourc, V *dest, double w);
	int getNumVertex() const;
	vector<V *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(V *s);
	bool aStarAlgorithm(V *origin, V *dest);
	void floydWarshallShortestPath();
	vector<V> getfloydWarshallPath(const V &origin, const V &dest) const;
	vector<V *> dijkstra(V *origin, V *dest);
	vector<V *> aStar(V *origin, V *dest);

	//other algorithms
	vector<V*> heldKarpAlgorithm(vector<V*> dest);
	pair<double, vector<V*>> heldKarpAlgorithm(V* dest, vector<V*> places);
};

template<class V, class E>
Graph<V, E>::Graph() {
	V::counter = 0;
}

template<class V, class E>
int Graph<V, E>::getNumVertex() const {
	return vertexSet.size();
}

template<class V, class E>
vector<V *> Graph<V, E>::getVertexSet() const {
	return vertexSet;
}

///*
// * Auxiliary function to find a vertex with a given content.
// */
//template <class V, class E>
//V * Graph<V,E>::findVertex(const V &in) const {
//	for (auto v : vertexSet)
//		if (v->info == in)
//			return v;
//	return NULL;
//}
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
	if (inVertexSet(in) != NULL)
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
bool Graph<V, E>::addEdge(V *sourc, V *dest, double w) {
	if (!inVertexSet(sourc) || !inVertexSet(dest))
		return false;

	sourc->addEdge(dest, w);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

template<class V, class E>
void Graph<V, E>::dijkstraShortestPath(V *origin) {
	MutablePriorityQueue<V> g;

	for (V * v : this->vertexSet) {
		v->dist = DBL_MAX;
		v->path = NULL;
	}

	V * node = origin;

	node->dist = 0;
	g.insert(node);

	while (!g.empty()) {
		node = g.extractMin();

		node->inClosedSet = true;

		for (E e : node->adj) {
			//Past distance
			double d = e.dest->dist;

			if (node->dist + e.weight < d) {

				//New calculated distance
				double newT = node->dist + e.weight;

				e.dest->dist = newT;
				e.dest->path = node;

				//Se ainda não estava na fila insere-se
				if (d == DBL_MAX)
					g.insert(e.dest);
				else
					g.decreaseKey(e.dest);
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
		std::cout << "\n\nId: " << v->getId() << std::endl;
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
	for (int i = 0; i < vertexSet.size(); i++) {
		for (int j = 0; j < vertexSet.size(); j++) {
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
	for (int k = 0; k < vertexSet.size(); k++) {
		for (int i = 0; i < vertexSet.size(); i++) {
			for (int j = 0; j < vertexSet.size(); j++) {
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

	/*
	// for tests
	for (int i = 0; i < weights.size(); i++) {
		cout << vertexSet.at(i)->getId();
		for (int j = 0; j < weights[0].size(); j++) {
			if (weights[i][j] == INF)
				cout << "INF" << " ";
			else
				cout << weights[i][j] << " ";
		}
		cout << endl;
	}
*/
}

template<class V, class E>
vector<V> Graph<V, E>::getfloydWarshallPath(const V &orig,
		const V &dest) const {

	int oIndex = this->findVertex(orig)->index;
	int dIndex = this->findVertex(dest)->index;

	vector<int> path;

	if (next.at(oIndex).at(dIndex) == INT_MAX)
		return path;

	path.push_back(oIndex + 1);

	while (oIndex != dIndex) {
		oIndex = next.at(oIndex).at(dIndex);
		path.push_back(oIndex + 1);
	}

	return path;

}

/*****************TRAVELING SALESMAN PROBLEM ALGORITHMS**********************/

template<class V, class E>
vector<V*> Graph<V, E>::heldKarpAlgorithm(vector<V*> dest) {

	/* Initializing the auxiliar memory structure */

	this->hkAuxMemory.clear();
	map<vector<V*>, pair<double, vector<V*>>> m;
	for (int i = 0; i < vertexSet.size(); i++) {
		hkAuxMemory.push_back(m);
	}

	this->floydWarshallShortestPath();

	for (int i = 0; i < dest.size(); i++) {
		hkAuxMemory[i][ { }].first = weights[dest.at(0)->index][i];
	}

	//this->hkAuxMemory = vector<map<vector<V*>, pair<double, vector<V*>>>>(dest.size(), m);
	vector<V*> copyDest = dest;
	copyDest.erase(copyDest.begin());
	hkBegin = dest.at(0);

	pair<double, vector<V*>> p = this->heldKarpAlgorithm(hkBegin, copyDest);

	if (p.first == INF)
	cout << "O Percurso escolhido é impossível" << endl;
	else
	{
		cout << "O peso de todo o percurso é: " << p.first << endl;
		cout << "O percurso é: ";
		for (int i = 0; i < p.second.size() -1; i++) {
			cout << p.second.at(i)->index << " - ";

		}
		cout << p.second.at(p.second.size()-1)->index;
	}

}

template<class V, class E>
pair<double, vector<V*>> Graph<V, E>::heldKarpAlgorithm(V* dest,
		vector<V*> places) {
	pair<double, vector<V*>> p;

	try {
		p = hkAuxMemory[dest->index].at(places);

	} catch (out_of_range &e) {
		if (places.empty()) {

			p.first = weights[hkBegin->index][dest->index];
			p.second.push_back(hkBegin);
			p.second.push_back(dest);
		} else {
			pair<double, vector<V*>> auxResult;
			int bestNextLocation = 0;
			double bestWeight = INF;

			for (int i = 0; i < places.size(); i++) {

				vector<V*> nextPlaces = places;
				V* nextDest = nextPlaces.at(i);
				if (nextPlaces.size() > 1)
					nextPlaces.erase(nextPlaces.begin() + i);
				else
					nextPlaces = {};
				auxResult = heldKarpAlgorithm(nextDest, nextPlaces);
				double tempWeight = weights[nextDest->index][dest->index]
						+ auxResult.first;

				if (tempWeight < bestWeight) {
					bestNextLocation = i;
					bestWeight = tempWeight;
				}

			}
			vector<V*> finalPath = auxResult.second;
			finalPath.push_back(dest);
			//initializing p
			p.first = bestWeight;
			p.second = finalPath;

		}
	}
	//the wanted pair is already in the data structure
	return p;
}
