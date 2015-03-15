#include <iostream>
#include <vector>
#include <queue>
using namespace std;
//NODE 0 is reserved
int N, M, K; //Set by io.
int e_min , g_min , f_min; //Set by io.
int n_vars , n_clauses;
vector< vector< bool > > edges; //Stores the adjacency matrix
std::deque<string> permutations;
int e(int i, int l); // =EDGES[i][j]
int g(int i,int j);  // =variables
int f(int i,int p,int q); // = representation of g[i][j] and some other things

enum MODE{
	PRINT_DEBUG, PRINT_ENCODING
};

#include "io_1.cpp"


/*
Clause numbering :
	0 => False
	1-g^2 => edge[i][j]
	g^2 + ( 1 - gk ) => g[i][p]
	g^2 + gk + (1-gkk) => x[i][p][q]

		HENCE MAPPING :
		e[i][j] = i*g + j;
		if ( i*g+j < g^2) then (i*g + j)%g 
	
*/

void eval_min();
void eval_nvars_nclauses();

void make_variables();
void make_nonempty_subgraph(); // k_AND(i_OR(g[i][k]))
void make_node_in_graph();	   // i_AND(k_OR(g[i][k]))
void make_complete_graph();    // ijk_AND( !g[i][k] or !g[j][k] or e[i][j])
void make_notsubgraph();	   // p_AND(q_AND( i_OR( fipq )))
void make_edge_in_graph(); 		// if( ed[i][j] ) then k_OR( g[i][k] and g[j][k] )

//Wrong functions
void make_edge_in_graph_wrong();// if(e[i][j]) then { g[x1][1] + g[x2][2] .... g[xg][k]} | xi belongs to {i,j}


void generate_clauses();

int main(int argc, char** argv) {
	input_1();
	//updatePaths(paths); //Unnecessary.
	eval_min();
	eval_nvars_nclauses();
	generate_clauses();
	return 0;
}

void eval_min() {
	//e_min = 0;
	g_min = 0;
	f_min = g_min + K*N;
}

void eval_nvars_nclauses() {
	n_vars = n*K*K + // f[i][p][q]
			n*K;  // g[i][k]
			//+ n*n; // e[i][j]
	n_clauses = //n*n + //e[i][j] 
			n*K*K + // f[i][p][q]
			K + //nonempty_subgraph
			N + //every node belongs to some subgraph
			M*K + //complete subgraphs
			K*K + //no two subgraphs are subgraphs of each other
			0; //TODO : how many clauses for each edge in some subgraph
			 
}
//Controls the loop itself.
void generate_clauses() {
	generate_permutations();
	printf("p cnf %d %d\n" , n_vars , n_clauses);
	make_variables();
	make_nonempty_subgraph();
	make_node_in_graph();
	make_complete_graph();
	make_edge_in_graph_wrong();
	make_notsubgraph();
}

void make_variables() {
	// e[i][j] -> 
	// for(int i=1; i<N+1; ++i) {
	// 	for(int j=1; j<N+1; ++j) {
	// 		if ( edges[i][j] ) {
	// 			printf("%d 0\n", e(i,j));
	// 		} else {
	// 			printf("%d 0\n", -e(i,j));
	// 		}
	// 	}
	// }
	//Reserve numbers for g[i][j].
	for( int i = 1; i<(N+1) ; ++i) {
		for(int p = 0; p<K; ++p) {
			for(int q = 0; q<K;++q) {
				int fipq = f(i , p , q);
				int gip = g( i , p );
				int giq = g( i , q );
				printf("%d %d %d 0\n", -fipq , gip , -giq);
				printf("%d %d 0\n" , -gip , fipq);
				printf("%d %d 0\n" ,  giq , fipq);
			}
		}
	}
} //Assert : Variables are ready.

void make_nonempty_subgraph() {
	//k_AND(i_OR(g[i][k]))
	for( int k = 0; k<K ; ++k) {
		//i_OR(g[i][k]);
		for( int i =1 ; i<(N+1) ; ++i ) {
			printf("%d ", g(i,k));
		}
		printf("0\n");
	}
}

void make_node_in_graph() {
	//i_AND(k_OR(g[i][k]))
	for(int i=1; i<(N+1); ++i) {
		for( int k=0; k<K; ++k) {
			printf("%d " , g(i,k));
		}
		printf("0\n");
	}
}

void make_complete_graph() {
	//ijk_AND( !g[i][k] or !g[j][k] or e[i][j])
	for(int i=1; i<(N+1) ; ++i) {
		for(int j=1; j< (N+1); ++j) {
			for(int k=0; k<K; ++k) {
				if ( !edges[i][j] ) {
					printf("%d %d 0\n" , -g(i,k) , -g(j,k) );
				}
			}
		}
	}
}

void make_notsubgraph() {
	//p_AND(q_AND( i_OR( fipq )))
	for(int p=0; p<K; ++p) {
		for(int q=0; q<K; ++q) {
			for(int i=0; i<(N+1); ++i) {
				printf("%d " , f(i,p,q));
			}
			printf("0\n");
		}
	}
}

void generate_permutations() {
	permutations.push_front("");
	while(true) {
		string present = permutations.front();
			if ( present.size() == K ) {
				break;
			}
			permutations.pop_front(); //give me an extract function dammit.
		permutations.push_back(present + "i");
		permutations.push_back(present + "j");
	}
	//generate permutations here.

}

void make_edge_in_graph_wrong() {
	// if(e[i][j]) then { g[x1][1] + g[x2][2] .... g[xg][k]} | xi belongs to {i,j}

	for(int i=1; i<(N+1); ++i) {
		for(int j=1; j<(N+1); ++j) {

			if( edges[i][j] ) {
				for(int p=0; p<permutations.size(); ++p) { //Each of (2**k) permutations
					
					for(int pos=0; pos< permutations.front().size() ; ++pos) {
						if( permutations[p][pos] == 'i' ) {
							printf("%d " , -g( i , pos ));
						} else {
							printf("%d " , -g( j , pos ));
						}
					} 
					printf("0\n");
				
				}
			}
		}
	}
}

void make_edge_in_graph() {
	for(int i=1; i<(N+1); ++i) {
		for(int j=1; j<(N+1); ++j) {
			if( edges[i][j] ) {
			}
		}
	}

}


int g(int i , int k) {
 	//Saved by k-major.
 	return k*N + i + g_min;
}

int f( int i , int p , int q) {
	return q*(N*K) + p*N + i + f_min;
}

int e( int i , int j) {
	 return (i-1)*N + j +e_min ; // + 1; // however, 1 <= i,j <= N
} //Max = N*N + N + 1
