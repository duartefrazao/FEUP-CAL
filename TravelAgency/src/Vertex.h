#ifndef VERTEX_H_
#define VERTEX_H_

#include "MutablePriorityQueue.h"
#include <iostream>

template <class V, class E> class Graph;
template <class T> class Edge;

template <class E>
class Vertex {
	typedef  double (*Heuristic)(Vertex<E>* v1, Vertex<E> *v2);

	static int counter;
	const int index;
	int auxIndex;

	bool inClosedSet;         	 	// auxiliary field
	bool inOpenSet;

	int queueIndex = 0; 		// required by MutablePriorityQueue

protected:
	double dist;
	double f;
	double h;
	double g;
	void addEdge(Vertex<E> *dest, double w);
	std::vector<E> adj;  // outgoing edgess

public:
	Vertex<E> *path = NULL;
	Vertex();
	bool operator<(Vertex<E> & vertex) const;	// required by MutablePriorityQueue
	double getDist() const;
	Vertex *getPath() const;
	std::vector<E> getAdj();
	double calculateF(Heuristic h, Vertex<E> *dest);
	void updateF(Heuristic h, Vertex<E> *dest);
	double getF() const;
	void setF(double f);

	template <class V, class U> friend class Graph;
	template <class T> friend class MutablePriorityQueue;
};

template <class E>
int Vertex<E>::counter= 0;

template <class E>
Vertex<E>::Vertex(): index(counter++) {}


template <class E>
void Vertex<E>::addEdge(Vertex<E> *d, double w) {
	std::cout << "Vertex\n";
	adj.push_back(E(d, w));
}

template <class E>
bool Vertex<E>::operator<(Vertex<E> & vertex) const {
	return this->f < vertex.f;
}

template <class E>
double Vertex<E>::getDist() const {
	return this->dist;
}

template <class E>
Vertex<E> *Vertex<E>::getPath() const {
	return this->path;
}

template <class E>
vector<E> Vertex<E>::getAdj(){
	return this->adj;
}

template <class E>
double Vertex<E>::calculateF(Heuristic heuristic, Vertex<E> *dest){

	double g = dist;
	double h = heuristic(this,dest);
	double f = g + h;

	return f;
}

template <class E>
inline double Vertex<E>::getF() const {
	return f;
}
template <class E>
inline void Vertex<E>::setF(double f) {
	this->f = f;
}

template <class E>
void Vertex<E>::updateF(Heuristic heuristic, Vertex<E> *dest){

	this->g = dist;
	this->h = heuristic(this,dest);
	this->f = g + h;
}

#endif /* VERTEX_H_ */
