/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include "MutablePriorityQueue.h"
#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <array>
#include <climits>


template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	static int counter;
	const int index;
	T info;                // contents
	std::vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	std::vector<Edge<T>> getAdj();
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

template <class T>
int Vertex<T>::counter= 0;

template <class T>
Vertex<T>::Vertex(T in): info(in), index(counter++) {}


/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
	Vertex<T>* getDest();
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}

template <class T>
Vertex<T>* Edge<T>::getDest(){
	return this->dest;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

template <class T>
vector<Edge<T>> Vertex<T>::getAdj(){
	return this->adj;
}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
	vector<vector<T>> m;
	vector<vector<T>> next;

public:
	Graph<T> ();
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template <class T>
Graph<T> ::Graph(){
	Vertex<T>::counter= 0;
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}

/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	MutablePriorityQueue<Vertex<T>> g;

	for(Vertex<T> * v:this->vertexSet){
		v->dist =DBL_MAX;
		v->path=NULL;
	}

	Vertex<T> * node =this->findVertex(origin);

	node->dist= 0;
	g.insert(node);



	while(!g.empty())
	{
		node = g.extractMin();

		node->visited=true;

		for(Edge<T> e:node->adj)
		{
			//Past distance
			double d = e.dest->dist;

			if(node->dist + e.weight < d){

				//New calculated distance
				double newT = node->dist + e.weight;

				e.dest->dist = newT ;
				e.dest->path = node;

				//Se ainda não estava na fila insere-se
				if(d== DBL_MAX ) g.insert(e.dest);
				else g.decreaseKey(e.dest);
			}


		}
	}



}


template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;

	Vertex<T> * v = this->findVertex(dest);

	while(v->info!= origin){
		res.push_back(v->info);
		v= v->path;
	}
	res.push_back(v->info);

	vector<T> resFinal;

	for(int i = res.size()-1; i>=0;i--) resFinal.push_back(res.at(i));

	return resFinal;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {

}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {

	//Initialization
	for(Vertex<T> * v:this->vertexSet){
		v->dist =DBL_MAX;
		v->path=NULL;
	}

	//First node
	Vertex<T> * node =this->findVertex(orig);
	node->dist= 0;

	int numVertices = this->vertexSet.size();

	//Relax each edge |V-1| times
	while(numVertices-->1)
	{
		//Relaxation step
		for(Vertex<T>* V:this->vertexSet)
		{
			for(Edge<T> E: V->adj)
			{
				//Past distance
				double d = E.dest->dist;

				if(V->dist + E.weight < d){

					//New calculated distance
					double newT = V->dist + E.weight;

					E.dest->dist = newT ;
					E.dest->path = V;
				}
			}
		}
	}




}



/**************** All Pairs Shortest Path  ***************/


template<class T>
void Graph<T>::floydWarshallShortestPath() {

	vector< T> line(vertexSet.size(),INT_MAX);
	vector<vector<T>>  newV (vertexSet.size(),line);

	vector< T> lineV(vertexSet.size(),NULL);
	vector<vector<T>>  newNext (vertexSet.size(),line);

	//Initialize array
	for(int i = 0; i < vertexSet.size();i++)
	{
		for(int j = 0; j < vertexSet.size();j++){
			j== i ? newV.at(i).at(j) =0:newV.at(i).at(j) = INT_MAX;
		}
	}


	//Add weights from edges
	for(Vertex<T>* V:this->vertexSet)
	{
		for(Edge<T> E:V->adj)
		{
			newV.at(V->index).at(E.dest->index) = E.weight;
			newNext.at(V->index).at(E.dest->index)= E.dest->index;
		}
	}



	//Algorithm
	for(int k = 0; k  <vertexSet.size();k++)
	{
		for(int i = 0; i  <vertexSet.size();i++)
		{
			for(int j = 0; j  <vertexSet.size();j++)
			{
				if(newV.at(i).at(j) >newV.at(i).at(k) +newV.at(k).at(j) && newV.at(i).at(k) !=INT_MAX && newV.at(k).at(j) != INT_MAX){
					newV.at(i).at(j)= newV.at(i).at(k) + newV.at(k).at(j);
					newNext.at(i).at(j)=newNext.at(i).at(k);
				}
			}
		}
	}

	m= newV;
	next=newNext;



}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{


	int oIndex=this->findVertex(orig)->index;
	int dIndex=this->findVertex(dest)->index;

	vector<int> path;

	if(next.at(oIndex).at(dIndex) == NULL) return path;

	path.push_back(oIndex+1);

	while(oIndex!= dIndex){
		oIndex= next.at(oIndex).at(dIndex);
		path.push_back(oIndex+1);
	}

	return path;

}

#endif /* GRAPH_H_ */
