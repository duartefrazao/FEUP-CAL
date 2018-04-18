/*
 * tspSolver.h
 *
 *  Created on: 9 Apr 2018
 *      Author: duarte
 */

#include "Location.h"

class tspSolver {
public:
	tspSolver(Graph<Location,Link> * g, Location* s, Location* d, vector<Location *> p);
	virtual ~tspSolver();
	bool nearestNeighbour(Location* s, Location* d, vector<Location *> p);
	vector<Location*> removeLocation(vector<Location *> p,Location *next);
	bool isPossible(Location * next, vector<Location*> p, Location *  s);
	void twoOpt();
	void duartegoritmo();
	double heuristic(Location * l2);
	double heuristic2(Location* l1, Location* l2);
	void formatPath();
	bool bfs(Location* s, Location* d);
	vector<Location*> getPath();
	vector<Location*> optSwap(int i,int k);
	double calculateTotalDistance(vector<Location*> route);
	bool haveConnection(Location* a, Location* b);
	bool solveTSPGreedy();
	bool solveTSPexperiments();
	const vector<Location*>& getFinalPath() const;
	void setFinalPath(const vector<Location*>& finalPath);

private:
	vector<Location * > finalPath;
	double currentBest;
	Graph<Location, Link> *graph;
	Location * source;
	Location * dest;
	vector<Location *> poi;
	vector<Location *> path;
};

