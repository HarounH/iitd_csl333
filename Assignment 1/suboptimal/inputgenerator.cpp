#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

int alphabetsize= 	4;
int numseq		=	5;
int cc			=	2;
int maxlength 	= 	50;
int minlength   =   5;
int mincost		=	1;
int maxcost		= 	4;
float _time 	= 	10.0;
vector< vector<float> > mc;


int main(int argc , char** argv) { //generates a file of name argv[1] 
	fstream writer; //Create a writer.
		writer.open(argv[1] , fstream::out);
	writer << _time << "\n";
	writer << alphabetsize << "\n";

	//Write alphabets
		for(int i = 'A' ; ((i<= (int)'Z') && ((i-'A')< alphabetsize-1 ) ); ++i) {
			writer << (char)i << ", ";

		}
		if( alphabetsize > 26) {	
			for(int i = 'a' ; ((i<= (int)'z') && ((i-'a')< alphabetsize-26-1 )); ++i) {
				writer << (char)i << ", ";
			}
		}
		if(alphabetsize<=26)
			writer << (char)('A' + alphabetsize-1);
		else
			writer << (char)('a' + alphabetsize-26-1);

		writer << "\n";

	writer << numseq << "\n";

	//Writer Sequences;
	srand(time(NULL));
	for(int i=0; i<numseq; ++i) {
		int length = minlength + rand()%(maxlength-minlength);
		if (alphabetsize>26) {
			for(int i=0; i<length; ++i) {
				int offset = rand()%(2*alphabetsize);
				if( offset<= alphabetsize/2 ) {
					char c = 'A' + offset;
					writer << c;
				} else {
					char c = 'a' + offset-26;
					writer << c;
				}
			}
		} else {
			for(int i=0; i<length; ++i) {
				int offset = rand()%alphabetsize;
				char c = 'A' + offset;
				writer << c;
			}
		}
		writer << "\n";
	}

	writer << cc << "\n";

	//Write cost matrix.
		//prepare vector of costs.
		mc.resize(alphabetsize+1);
		for(int i=0; i<mc.size(); i++) {
			mc[i].resize(alphabetsize+1);
		}

		//Give values
	for(int r = 0; r<= alphabetsize; ++r) {
		for(int c=r; c<=alphabetsize; ++c) {
			mc[r][c] = (r==c)?(0):( mincost + rand()%(maxcost-mincost) );
			mc[c][r] = mc[r][c];
		}
	}

	//Print out the matrix;
	for(int r=0; r<alphabetsize+1; ++r) {
		for(int c= 0 ; c<alphabetsize+1; ++c) {
			writer << mc[r][c] << " ";
		}
		writer << "\n";
	}

	mc.resize(0);
	writer << "#\n";
	writer.close();
	return 0;
}