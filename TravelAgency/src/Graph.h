#include "Vertex.h"
#include "Edge.h"
#include "MutablePriorityQueue.h"
#include <vector>
#include <limits.h>
#include <cfloat>
#include "Location.h"
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
	double heldKarpAlgorithm(V *origin, vector<V*> dest);
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
template <class V, class E>
bool Graph<V,E>::aStarAlgorithm(V *origin, V *dest) {

	MutablePriorityQueue<V> openSet;

	for(auto vertex : vertexSet){
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

	while(!openSet.empty()){

		V * v = openSet.extractMin();
		v->inClosedSet = true;
		v->inOpenSet = false;
		std::cout << "\n\nId: " << v->getId() << std::endl;
		if(v == end){
			return true;
		}

		for(E edge : v->adj){
			V * destV = edge.getDest();

			if(destV->inClosedSet)
				continue;

			if(!destV->inOpenSet){
				destV->dist = v->dist + edge.weight;
				destV->path = v;
				destV->updateF(heuristic, end);
				destV->inOpenSet = true;
				openSet.insert(destV);
				continue;
			}

			if(destV->calculateF(heuristic, end) < destV->f){
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

	vector<double> line(vertexSet.size(), INT_MAX);
	vector<vector<double>> newV(vertexSet.size(), line);

	vector<int> lineV(vertexSet.size(), NULL);
	vector<vector<int>> newNext(vertexSet.size(), lineV);

	//Initialize array
	for (int i = 0; i < vertexSet.size(); i++) {
		for (int j = 0; j < vertexSet.size(); j++) {
			j == i ? newV.at(i).at(j) = 0 : newV.at(i).at(j) = INT_MAX;
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
										j) && newV.at(i).at(k) != INT_MAX
										&& newV.at(k).at(j) != INT_MAX) {
					newV.at(i).at(j) = newV.at(i).at(k) + newV.at(k).at(j);
					newNext.at(i).at(j) = newNext.at(i).at(k);
				}
			}
		}
	}

	weights = newV;
	next = newNext;

	/*
	 *
	 * for tests
	 for (int i = 0; i < weights.size(); i++) {
	 cout << vertexSet.at(i)->getId();
	 for (int j = 0; j < weights[0].size(); j++) {
	 if (weights[i][j] == INT_MAX)
	 cout << "INF" << " ";
	 else cout << weights[i][j] <<  " ";
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

	if (next.at(oIndex).at(dIndex) == NULL)
		return path;

	path.push_back(oIndex + 1);

	while (oIndex != dIndex) {
		oIndex = next.at(oIndex).at(dIndex);
		path.push_back(oIndex + 1);
	}

	return path;

}

/*****************TRAVELING SALESMAN PROBLEM ALGORITHMS**********************/


/*
template<class V, class E>
double Graph<V, E>::heldKarpAlgorithm(V * dest, vector<V*> interm) {



	 //Computing all pairs shortest path
	 this->floydWarshallShortestPath();


	if (interm.size() == 1) {

		return weights[dest->index][interm[0]->index];
	}

	else {
		//auxiliar variables
		double minWeight = INF;
		unsigned int leadingVertex = 0;
		vector<V*> auxDest;
		V* auxOrigin;

		for (unsigned int i = 0; i < interm.size(); i++) {
			auxDest = dest;
			auxDest.erase(auxDest.begin() + i);
			double result = heldKarpAlorithm(interm.at(i), auxDest);
			minWeight = (result < minWeight) ? result : minWeight;
		}

		return minWeight;
	}

}

*/
