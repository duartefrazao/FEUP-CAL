

#ifndef EDGE_H_
#define EDGE_H_

template <class T> class Vertex;

template <class V>
class Edge {
	V * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(V *d, double w);
	template <class U, class E> friend class Graph;
	template <class T>friend class Vertex;
	V* getDest();
	void setWeight(double w);

	double getWeight() const {
		return weight;
	}
};

template <class V>
Edge<V>::Edge(V *d, double w): dest(d), weight(w) {}

template <class V>
V* Edge<V>::getDest(){
	return this->dest;
}

template <class V>
void Edge<V>::setWeight(double w){
	this->weight = w;
}

#endif /* EDGE_H_ */
