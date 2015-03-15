//part1_v2.cpp
#include <stdio.h>
#include <vector>
using namespace std;
//NODE 0 is reserved
int N, M, K; //Set by io.
int g_min , f_min , h_min; //Set by eval_min()
int n_vars , n_clauses;
vector< vector< bool > > edges; //Stores the adjacency matrix

enum MODE{
	PRINT_DEBUG, PRINT_ENCODING
};

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "io_1.cpp"

void eval_min();
int g( int i , int p ); 		//Trivial - is node_i in subgraph_p ?
int f( int i , int p , int q);  //Is node_i present in subgraph_p but not in subgraph_q
int h( int i , int j , int p);  //Are node_i and node_j both present in subgraph_p

void eval_nvars();
void eval_nclauses();
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void make_variables();			// relations between g,f,h
void make_nonempty_subgraph(); 	// k_AND(i_OR(g[i][k]))
void make_node_in_graph();		// i_AND(k_OR(g[i][k]))
void make_complete_graph();		// ijk_AND( !g[i][k] or !g[j][k] or e[i][j])
void make_not_subgraph();		// p_AND(q_AND( i_OR( fipq )))
void make_edge_in_graph();		// ij_AND( if( edges[i][j] )) then k_OR( h[i][j][k] )
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

int main(int argc , char** argv) {
	input_1();
	eval_min();
	eval_nvars();
	eval_nclauses();
	printf("p cnf %d %d\n" , n_vars , n_clauses);
	make_variables();
	make_nonempty_subgraph();
	//make_node_in_graph(); Not necessary
	make_complete_graph();
	make_not_subgraph();
	make_edge_in_graph();
	return 1;
}

int g(int i, int p) { // g[i][k] -> node_i belongs to subgraph_p
	return g_min + p*N + i;
}
int f(int i , int p , int q) { // f[i][p][q] <-> (g[i][p] and (-g[i][q])) <---> ( -fipq + gip )( -fipq + -giq )( -gip + giq + fipq )
	return f_min + q*(N*K) + p*(N) + i;
}
int h(int i , int j , int p) { //h[i][j][p] <-> (g[i][p] and g[j][p]) <---> ( -hijp + gip )( -hijp + gjp )( -gip + -gjp + hijp)
	return h_min  + p*(N*N) + (j-1)*N + i;
}
void eval_min() {
	g_min = 0; 
	f_min = g_min + N*K;
	h_min = f_min + N*K*K;
	return;
}
void eval_nvars() {
	n_vars = N*K + 	//g[i][k]
			N*K*K + //f[i][p][q]
			N*N*K; 	//h[i][j][q]
}
void eval_nclauses() {
	//TODO:
	n_clauses = 3*N*K*(K-1) + // clauses to fix fipq
				3*N*(N-1)*K + // clauses to fix hijq
				K 	+ // clauses in make_nonempty_subgraph
				//N 	+ // clauses in make_node_in_graph
				(((N*(N-1)/2) + -M)*K) + // clauses in make_complete_graph()
				K*(K - 1)	+ // clauses for make_not_subgraph
				M;		  // clauses for make_edge_in_subgraph
}	
void make_variables() {
	//no constraints on g[i][k]
	// f[i][p][q] <-> (g[i][p] and (-g[i][q])) <---> ( -fipq + gip )( -fipq + -giq )( -gip + giq + fipq )
	for(int i = 1; i<(N+1) ; ++i) {
		for(int p=0; p<K; ++p) {
			for(int q=0; q<K; ++q) {
				if( p!=q) {
					int fipq 	= f(i,p,q);
					int gip 	= g(i,p);
					int giq		= g(i,q);
					printf("%d %d 0\n" , -fipq , gip);
					printf("%d %d 0\n" , -fipq , -giq);
					printf("%d %d %d 0\n" , -gip , giq , fipq); 
				}
			}
		}
	} //fipq done.
	//cout << " f wali things \n";
	//h[i][j][p] <-> (g[i][p] and g[j][p]) <---> ( -hijp + gip )( -hijp + gjp )( -gip + -gjp + hijp)
	for(int p = 0; p<K; ++p) {
		for(int i=1; i<(N+1) ; ++i) {
			for(int j=1; j<(N+1) ; ++j) {
				if (i!=j) {
					int hijp = h(i,j,p);
					int gip = g(i,p);
					int gjp = g(j,p);
					printf("%d %d 0\n" , -hijp , gip);
					printf("%d %d 0\n" , -hijp , gjp);
					printf("%d %d %d 0\n" , -gip , -gjp , hijp);
				}
			}
		}
	}
	//cout << " h wali things \n";
} // 3*N*K*(K-1) +  3*N*(N-1)*K
void make_nonempty_subgraph() { 	// k_AND(i_OR(g[i][k]))
	for(int k=0; k<K; ++k) {
		for(int i=1; i<(N+1); ++i) {
			printf("%d " , g(i,k));
		}
		printf("0\n");
	}
	//cout << "non empty subgraphs \n";
} // K clauses
void make_node_in_graph() {
	for(int i = 1; i<(N+1) ; ++i) {
		for(int k=0; k<K; ++k) {
			printf("%d " , g(i,k));
		}
		printf("0\n");
	}
	//cout << "nodes in graph \n";
} //N clauses
void make_complete_graph() { // ijk_AND( !g[i][k] or !g[j][k] or e[i][j])
	for(int i=1; i<(N+1) ; ++i) {
		for(int j=i+1; j<= N ; ++j) {
			for(int k = 0; k<K ; ++k) {
				if(!edges[i][j]) {
					printf("%d %d 0\n" , -g(i,k) , -g(j,k));
				}
			}
		}
	}
	//cout << "complete graphs \n";
} // ((N*(N-1)/2) + -M)*K
void make_not_subgraph() { //p_AND(q_AND( i_OR( fipq )))
	for(int p = 0; p<K; ++p) {
		for(int q=0; q<K ; ++q) {
			if ( p!= q){
				for(int i=1; i<(N+1); ++i) {
					printf("%d ", f(i,p,q));
				}
				printf("0\n");
			}		
		}
	}
	//cout << "not subgraph \n";
} // K*(K-1)
void make_edge_in_graph() { // ij_AND( if( edges[i][j] )) then k_OR( h[i][j][k] )
	for(int i=1; i<=N ; ++i) {
		for(int j=i+1; j<=N; ++j) {
			if( edges[i][j] ) {
				for(int k=0; k<K; ++k) {
					printf("%d " , h(i,j,k));
				}
				printf("0\n");
			}
		}
	}
	//cout << "edge in graph";
} // M clauses