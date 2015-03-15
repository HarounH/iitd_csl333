#ifndef FORMULATION_CPP
	#define FORMULATION_CPP
//Functions for printing.
void Problem::print() {
	cout << " time_limit = " << timeLimit << "\n";
	cout << " time_buffer = " << TIME_BUFFER << "\n";
	cout << " alphabet_size = " << aSize << "\n";
	cout << " alphabets = " ;
	for(int i=0; i< aSize; ++i) cout << idxToChar[i] << "@" << charToIdx[idxToChar[i]] << ", ";
		cout << "\n";
	cout << " num_sequences = " << k << "\n";
	cout << " sequences : \n" ;
	for(int i=0; i<k; ++i) cout << inSequences[i] << " of length = " << seqLengths[i] << "\n";

	cout << " cc = " << cc << "\n";
	for(int i=0; i<= aSize; ++i) {
		for(int j=0; j<=aSize; ++j) {
			cout << mc[i][j] << " ";
		}
		cout << "\n";
	}
}

void Problem::printState(State& s) {
	cout << "length = " << s.length << "\n";
	cout << "cost = " << s.cost << "\n";
	for(int i=0; i<k; ++i) {
		for(int j=0; j<s.length; ++j) {cout << s.sequences[i][j] ;} cout << "\n";
		// cout << "\t dashPos at : ";
		// for(int j = 0; j<s.dashPos[i].size();++j) {
		// 	cout << s.dashPos[i][j] << ",";
		// }
	}
}

double Problem::evalCost(State& s) {
	double cost = 0.0;
	for(int idx1 = 0; idx1 < k; ++idx1) {
		cost += cc*(s.dashPos[idx1].size());
		for(int idx2 = idx1+1; idx2 < k; ++idx2) {
			for(int pos = 0; pos<s.length; ++pos) {
				cost += mc[charToIdx[ s.sequences[idx1][pos] ]][ charToIdx[ s.sequences[idx2][pos] ] ];
			}
		}
	}
	return cost;
}

double Problem::evalChildCost_singleSwap(State& parent , State& child , int stringIdx , int swapIdx1 , int swapIdx2) {
	double delta_cost = 0.0;
	//Dont need to handle the change in number of dashes because there is no change in number of dashes
	for(int stringIdx2 = 0; stringIdx2<k; ++stringIdx2) {
		if (stringIdx2 != stringIdx) {
			delta_cost += mc[ charToIdx[child.sequences[stringIdx][swapIdx1]] ][ charToIdx[child.sequences[stringIdx2][swapIdx1]] ] - mc[ charToIdx[parent.sequences[stringIdx][swapIdx1]] ][ charToIdx[parent.sequences[stringIdx2][swapIdx1]] ];
			delta_cost += mc[ charToIdx[child.sequences[stringIdx][swapIdx2]] ][ charToIdx[child.sequences[stringIdx2][swapIdx2]] ] - mc[ charToIdx[parent.sequences[stringIdx][swapIdx2]] ][ charToIdx[parent.sequences[stringIdx2][swapIdx2]] ];
		} //Else pass
	}
	return parent.cost + delta_cost;
}

void Problem::init() { //Generates a random initState.
	uniform_int_distribution<int> lengthinit(minFinalLength , minFinalLength + ((maxFinalLength-minFinalLength)/k));
	initState.length = lengthinit(engine); //initializes the length of the initState.
	uniform_int_distribution<int> dashinit(0 , initState.length-1);
	initState.dashPos.resize(k);
	initState.sequences.resize(k);
	for(int idx = 0; idx<k; ++idx) {
		initDashes(idx , dashinit);
		initDeques(idx);
	}
	initState.cost = evalCost(initState);
}

void Problem::initDashes(int idx , uniform_int_distribution<int>& dashinit) { //Initializes the idx-th dashPos for the initState
	int n_dashes = initState.length - seqLengths[idx];
	initState.dashPos[idx].resize( n_dashes );
	for(int i=0; i< n_dashes; ++i) {
		initState.dashPos[idx][i] = (dashinit(engine)%(seqLengths[idx]));
	}
	sort( initState.dashPos[idx].begin() , initState.dashPos[idx].end());
}

void Problem::initDeques(int idx) { //initializes idx-th string for the initState
	int dposptr=0; // points to the next index in dashPos[idx] to be inserted.
	int sposptr=0; // points to the next index in inSequences[idx] to be inserted.
	for(int ptr	=0; ptr<initState.length ;++ptr) {		//points to the location in the deque to be inserted into.
		if ( (dposptr < initState.dashPos[idx].size()) && (initState.dashPos[idx][dposptr] + dposptr == ptr) ) {
			initState.sequences[idx].push_back('-');
			++dposptr;
		} else if (sposptr < seqLengths[idx]) {
			initState.sequences[idx].push_back(inSequences[idx][sposptr]);
			++sposptr;
		}
	}
}

void Problem::randomState(State& s , State& best) {
	s.length = generateRandomLength(best); //Creates a new length using the present best's length.
	s.cost = 0.0;
	s.dashPos.resize(k);
	s.sequences.resize(k);
	uniform_int_distribution<int> dashinit(0 , s.length-1);
	for(int idx = 0; idx<k; ++idx) {
		generateRandomDashPos(idx , dashinit , s);
		generateDeque(idx , s);
	}
	s.cost = evalCost(s);
}

int Problem::generateRandomLength(State& best) {
	// TODO:Insert cool function here.
	uniform_int_distribution<int> lengthinit(minFinalLength , minFinalLength + 2*minFinalLength);
	return lengthinit(engine);
}

void Problem::generateRandomDashPos(int idx , uniform_int_distribution<int>& dashinit , State& s) {
	int n_dashes = s.length - seqLengths[idx];
	s.dashPos[idx].resize(n_dashes);
	for(int i=0; i< n_dashes; ++i) {
		//Slightly biased dashPos.
		s.dashPos[idx][i] = (dashinit(engine)%(seqLengths[idx] + 1));
	}
	sort( s.dashPos[idx].begin() , s.dashPos[idx].end());	
}

void Problem::generateDeque(int idx , State& s) {
	int dposptr=0; // points to the next index in dashPos[idx] to be inserted.
	int sposptr=0; // points to the next index in inSequences[idx] to be inserted.
	s.sequences[idx].resize(0);
	for(int ptr	= 0; ptr < s.length ;++ptr) {		//points to the location in the deque to be inserted into.
		if ( (dposptr < s.dashPos[idx].size()) && (s.dashPos[idx][dposptr] + dposptr == ptr) ) {
			s.sequences[idx].push_back('-');
			++dposptr;
		} else if (sposptr < seqLengths[idx]) {
			s.sequences[idx].push_back(inSequences[idx][sposptr]);
			++sposptr;
		}
	}
}

void Problem::getNBD(State& state , vector<State>& nbd) {
	for(int idx = 0; idx < k; ++idx) {
		//Every dashPos represents how many sequence characters before that given dash.
		for(int didx = 0; didx < state.dashPos[idx].size(); ++didx) {
			//forwardSwap:
			if(state.dashPos[idx][didx] < seqLengths[idx]) { //There's still scope to increase this dashPos[idx][didx]
				State child = state;
				//Positions in the deque of characters.
				int seqpos1 = state.dashPos[idx][didx] + didx; //This index was swapped with something else
				int seqpos2 = seqpos1+1;
				while( (seqpos2 < child.sequences[idx].size()) && (child.sequences[idx][seqpos2] == '-')) {
					seqpos2++;
				}
				//swap the character with a dash.
				child.sequences[idx][seqpos1] = child.sequences[idx][seqpos2];
				child.sequences[idx][seqpos2] = '-';
				
				int swapWith = didx+1;
				int oldValue = child.dashPos[idx][didx];
				++child.dashPos[idx][didx];
				//sort dashpos[idx];
				while ( (swapWith < state.dashPos[idx].size()) && (state.dashPos[idx][didx] > state.dashPos[idx][swapWith])) {
					child.dashPos[idx][didx] = child.dashPos[idx][swapWith];
					child.dashPos[idx][swapWith] = oldValue;
					++swapWith;
				}
				child.cost = evalChildCost_singleSwap(state , child , idx , seqpos1 , seqpos2);
				nbd.push_back(child);
			}
			//backwardSwap:	
			if(state.dashPos[idx][didx] > 0) {
				State child = state;
				//Possible to backward swap
				int seqpos1 = state.dashPos[idx][didx] + didx;
				int seqpos2 = seqpos1-1;
				while ( (seqpos2 > 0) && ( child.sequences[idx][seqpos2] == '-' ) ) {
					seqpos2--;
				}

				child.sequences[idx][seqpos1] = child.sequences[idx][seqpos2];
				child.sequences[idx][seqpos2] = '-';
				
				int swapWith = didx-1;
				int oldValue = child.dashPos[idx][didx];
				--child.dashPos[idx][didx];

				while ( (swapWith>0) && (state.dashPos[idx][didx] < state.dashPos[idx][swapWith])) {
					child.dashPos[idx][didx] = child.dashPos[idx][swapWith];
					child.dashPos[idx][swapWith] = oldValue;
					--swapWith;
				}
				child.cost = evalChildCost_singleSwap(state , child , idx , seqpos1 , seqpos2);
				nbd.push_back(child);
			}
		}
	}
}
#endif