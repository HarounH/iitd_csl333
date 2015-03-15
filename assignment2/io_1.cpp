#ifndef IO_1_CPP
	#define IO_1_CPP

#include <iostream>
#include <stdio.h>
void input_1() {
	
	cin >> N; //number of vertices
	cin >> M; //number of edges
	cin >> K; //number of desired subgraphs
	//NODE 0 is reserved.

	edges.resize(N+1);
	for(int idx = 0; idx < N+1; ++idx) edges[idx].resize(N+1);

	for( int e = 0; e<M; ++e) {
		int from, to;
		cin >> from;
		cin >> to;
		edges[from][to] = true;
		edges[to][from] = true;
		//adjlist[from].push_back(to);
		//adjlist[to].push_back(from);
	}

} //Working.

void output_1(MODE mode) {

	if(mode == PRINT_DEBUG) {
		printf(" N=%d \n M=%d \n K=%d\n", N, M, K);
		for(int r=1; r<N+1; ++r) {
			for(int c=1; c<N+1; ++c) {
				cout << edges[r][c] << " ";
			}
			cout << "\n";
		}
	} else if( mode == PRINT_ENCODING) {

	}
}
#endif