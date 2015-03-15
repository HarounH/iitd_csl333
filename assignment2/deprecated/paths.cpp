#include <iostream>
#include <set>
#include <queue>
void updatePaths(vector< vector<bool> >& p) {
	// p is the isReachable() matrix.
	//use DP to optimize.
	//Handle memory.
	p.resize(N+1);
	for(int i=0; i< N+1; ++i) p[i].resize(N+1 , false);
	vector<bool> explored(N+1 , false);
	//Start dfs-ing.

	//Use adjacency list for node i.
	for(int node = 1; node < N+1; ++node) {
		//For each node, check its adjacency list.
		bfs(node , p , explored);
	}
}

void bfs(int start , vector< vector<bool> >& p , vector<bool>& explored) {
	//Start doing bfs
	set<int> reachable;
	queue<int> fringe;
	reachable.push_back(start);
	fringe.push_back(start);
	if ( !explored[start] ) {
		int prevsize = 0;
		while ( prevsize != reachable.size() ) {
			prevsize = reachable.size(); 
			//Do bfs.
			int nextNode = fringe.pop();
			//Insert all of its kids into the fringe and also into the reachable matrix.
			for(int it = 0; it< adjlist[nextNode].size() ; ++i) {
				reachable.insert( adjlist[nextNode][i] );
				fringe.push( adjlist[nextNode][i] ); //Do this if the node is not present in reachable?
			}
		}
		set<int> iterator it1;
		set<int> iterator it2;
		for(int it1 = 1 ; idx1 < reachable.size();
	}
}