#include <algorithm>

void getDPInit();

void init( MODE mode) {
	if (mode == INIT_APPEND_BACK) {
		//length init
		//uniform_int_distribution<int> linit(minFinalLength , minFinalLength + ((maxFinalLength-minFinalLength)/k));
		length = minFinalLength + (rand()%( 1 + (int)((maxFinalLength-minFinalLength)/k)));
		dashpos.resize(k);
		for(int idx=0; idx < k; ++idx) {
			while( sequences[idx].size() < length ) {
				sequences[idx].push_back('-');
				dashpos[idx].push_back( seqLengths[idx] );
			}
		}
	} else if ( mode == INIT_RANDOM ) {
		//uniform_int_distribution<int> linit(minFinalLength , minFinalLength + ((maxFinalLength-minFinalLength)/k));
		length = minFinalLength + (rand()%( 1 + (int)((maxFinalLength-minFinalLength)/k)));
		//dash init
		dashpos.resize(k);
		//uniform_int_distribution<int> dinit( 0 , length);
		for(int idx = 0; idx<k; ++idx) {
			dashpos[idx].resize(0);
			//############################################################################# INSERTION BEGIN
			for(int didx = 0; didx < (length - seqLengths[idx]) ; ++didx) {
				dashpos[idx].push_back( (rand()%(seqLengths[idx] + 1)) ); // Because the valuesa at dashpos[idx][didx] represent how many chracters before the didx'th dash.
			}
			//###########################################################################SORTING DASHPOS BEGIN
			sort(dashpos[idx].begin() , dashpos[idx].end());
			//###########################################################################SORTING DASHPOS END
			for(int didx = 0; didx< length - seqLengths[idx]; ++didx) {
				sequences[idx].insert( sequences[idx].begin() + didx + dashpos[idx][didx] , 1, '-' ); //Insert dashes in the appropriate locations
			}
			//############################################################################## INSERTION END
		}
	} else if ( mode == INIT_DP ) {
		getDPInit();
	}
	cost = evalCost();
	initcost = cost;
}

void getDPInit() {
	//IMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANT
	// 0 and 1 and 2 and 3 are 0=> STOP , 1=> parent is i-1,j-1 , 2=> i-1 ,j 3=> i,j-1 IMPORTANT 
	//IMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORTANTIMPORT
	//Dp on string_i and string_(i+1)
	int sidx=0;
	int maxLength = 0;
	dashpos.resize(k);
	//insequences contains characters.
	//sequences is what i'll make changes to.
	while ( sidx+1 < k ) {
		vector < vector<double> > A; //Stores the dp thingy.
		vector< vector<int> > B; //Stores the parenty thingy.
		int localLength = 0;
		B.resize(seqLengths[sidx]+1);
		A.resize(seqLengths[sidx]+1);

		B[0].resize(seqLengths[sidx+1] + 1);
		A[0].resize(seqLengths[sidx+1] + 1);
		
		//Initializations
		for( int i = 1; i <= seqLengths[sidx]; ++i) {
			B[i].resize(seqLengths[sidx+1] + 1 , 0);

			A[i].resize(seqLengths[sidx+1] + 1);
			A[i][0] = A[i-1][0] + cc + mc[charToIdx[(int) insequences[sidx][i-1]]][charToIdx[(int) '-']];
		
		}
		for( int i = 1; i <= seqLengths[sidx+1]; ++i) {
			A[0][i] = A[0][i-1] + cc + mc[charToIdx[(int) '-']][charToIdx[(int) insequences[sidx+1][i-1]]];
		}
		//DP
		for( int i=1; i <= seqLengths[sidx] ; ++i) {
			for( int j=1; j <= seqLengths[sidx+1] ; ++j) {
				double x = A[i-1][j-1] + mc[charToIdx[(int) insequences[sidx][i-1]]][charToIdx[(int) insequences[sidx+1][j-1]]];
				double y = A[i-1][j] + cc + mc[charToIdx[(int) insequences[sidx][i-1]]][charToIdx[(int) '-']];
				double z = A[i][j-1] + cc + mc[charToIdx[(int) '-']][charToIdx[(int) insequences[sidx+1][j-1]]];
				double w = min( x, min(y,z));
				if(w == x)	{
					B[i][j] = 1;
				} else if ( w==y ) {
					B[i][j] = 2;
				} else {
					B[i][j] = 3;
				}
				A[i][j]=w;
			}
		}
		//Reconstruction
		vector<int> result;
		int x = seqLengths[sidx];
		int y = seqLengths[sidx+1];
		while(B[x][y] != 0)
		{
			if(B[x][y] == 1)
			{
				x--;
				y--;
				result.push_back(1);
			}
			else if(B[x][y] == 2)
			{
				x--;
				y;
				result.push_back(2);
			}
			else if(B[x][y] == 3)
			{
				x;
				y--;
				result.push_back(3);
			}
		}
		sequences[sidx].resize(0);
		sequences[sidx+1].resize(0);
		dashpos[sidx].resize(0);
		dashpos[sidx+1].resize(0);
		//cout << "x & y = " << x << " " << y << "\n"; 
		if(x == 0)
			localLength = y;
		else
			localLength = x;
		for(int j = 0; j < x; j++){
			sequences[sidx].push_back(insequences[sidx][j]);
			sequences[sidx+1].push_back('-');
			dashpos[sidx+1].push_back(0);
		}
		for(int j = 0; j < y; j++){
			sequences[sidx+1].push_back(insequences[sidx+1][j]);
			sequences[sidx].push_back('-');
			dashpos[sidx].push_back(0);
		}
		for(int j = result.size() - 1; j >=0; j--)
		{
			if(result[j] == 1)
			{
				sequences[sidx].push_back(insequences[sidx][x]);
				sequences[sidx+1].push_back(insequences[sidx+1][y]);
				x++;
				y++;
			}
			else if(result[j] == 2)
			{
				sequences[sidx].push_back(insequences[sidx][x]);
				x++;
				sequences[sidx+1].push_back('-');
				dashpos[sidx+1].push_back(y);
			}
			else
			{
				sequences[sidx+1].push_back(insequences[sidx+1][y]);
				y++;
				sequences[sidx].push_back('-');
				dashpos[sidx].push_back(x);
			}
		}
		localLength += result.size();
		if(localLength > maxLength)
			maxLength = localLength;
		sidx += 2;
	}

		//Handling.
		//Handle lonely string.
	if ( sidx == k ) {
		//We're done.
	} else {
		//Handle lonely string.
		if(seqLengths[k-1] > maxLength)
			maxLength = insequences[k-1].size();
	}
	for(int i = 0; i < k; i++) {
		int s = maxLength - sequences[i].size();
		sequences[i].resize(maxLength, '-');
		dashpos[i].resize(dashpos[i].size() + s, seqLengths[i]);
	}
	//Handle generation of sequences.
	length = maxLength;
	cost = evalCost();
}