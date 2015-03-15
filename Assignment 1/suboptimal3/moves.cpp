
void getMoves( vector<Move>& moves ) { //Generates 2*k*length neighbours. Also stores the best neighbour yet in a global variable.
	for(int idx=0; idx<k; ++idx) {
		for(int didx = 0; didx < dashpos[idx].size(); ++didx) {
			//Forward swap
			if ( dashpos[idx][didx] < seqLengths[idx] ) { //it is possible to swap forward.
				Move move;
					move.newcost = 0.0;
					move.idx = idx;
					move.di = didx;
				int origDptr = dashpos[idx][didx] + didx;
				int newDptr  = dashpos[idx][didx] + didx; // + 1 will be taken care of by the while loop
				while ((newDptr < length) && (sequences[idx][newDptr]=='-')) {
					++newDptr;
				}
				move.origDptr = origDptr;
				move.newDptr  = newDptr;
				move.newcost  = cost + evalCost_move(move);
				//cout << " Move contents contain : \n" << " idx = " << move.idx << "\n" << " di = " << move.di << "\n" << " origdptr = " << move.origDptr << "\n" << " newdptr = " << move.newDptr << "\n";  
				if (( move.newDptr < length) && (move.origDptr < length) && (move.newDptr >=0) && (move.origDptr >=0)) {
					moves.push_back(move);
				}
			}
			//Backward swap
			if ( dashpos[idx][didx] > 0 ) { //it is possible to swap backward 
				Move move;
					move.newcost = 0.0;
					move.idx = idx;
					move.di = didx;
				int origDptr = dashpos[idx][didx] + didx;
				int newDptr  = dashpos[idx][didx] + didx; // -1 will be taken care of by the while loop.
				while ((newDptr > 0) && (sequences[idx][newDptr]=='-')) {
					--newDptr;
				}
				move.origDptr = origDptr;
				move.newDptr  = newDptr;
				move.newcost  = cost + evalCost_move(move);
				//cout << " Move contents contain : \n" << " idx = " << move.idx << "\n" << " di = " << move.di << "\n" << " origdptr = " << move.origDptr << "\n" << " newdptr = " << move.newDptr << "\n";
				if (( move.newDptr < length) && (move.origDptr < length) && (move.newDptr >=0) && (move.origDptr >=0)) {
					moves.push_back(move);
				}
			}
		}
	}
}

void getBestMove( Move& bestMove ) {
	bestMove.newcost = 0.0;
	for(int idx=0; idx<k; ++idx) {
		for(int didx = 0; didx < dashpos[idx].size(); ++didx) {
			//Forward swap
			if ( dashpos[idx][didx] < seqLengths[idx] ) { //it is possible to swap forward.
				Move move;
					move.newcost = 0.0;
					move.idx = idx;
					move.di = didx;
				int origDptr = dashpos[idx][didx] + didx;
				int newDptr  = dashpos[idx][didx] + didx + 1;
				while ((newDptr < length) && (sequences[idx][newDptr]=='-')) {
					++newDptr;
				}
				move.origDptr = origDptr;
				move.newDptr  = newDptr;
				move.newcost += cost + evalCost_move(move);
				if ( move.newcost < bestMove.newcost )  {
					bestMove.idx = move.idx;
					bestMove.di = move.di ;
					bestMove.origDptr = move.origDptr;
					bestMove.newDptr = move.newDptr;
					bestMove.newcost = move.newcost;
				}
			}
			//Backward swap
			if ( dashpos[idx][didx] > 0 ) { //it is possible to swap backward 
				Move move;
					move.newcost = 0.0;
					move.idx = idx;
					move.di = didx;
				int origDptr = dashpos[idx][didx] + didx;
				int newDptr  = dashpos[idx][didx] + didx - 1;
				while ((newDptr > 0) && (sequences[idx][newDptr]=='-')) {
					--newDptr;
				}
				move.origDptr = origDptr;
				move.newDptr  = newDptr;
				move.newcost += cost + evalCost_move(move);
				if ( move.newcost < bestMove.newcost )  {
					bestMove.idx = move.idx;
					bestMove.di = move.di ;
					bestMove.origDptr = move.origDptr;
					bestMove.newDptr = move.newDptr;
					bestMove.newcost = move.newcost;
				}
			}
		}
	}
}

void setState(Move& move) {
	cost = move.newcost;
	sequences[move.idx][ move.origDptr ] = sequences[move.idx][ move.newDptr ];
	sequences[move.idx][ move.newDptr ] = '-';

	int change = ((move.newDptr > move.origDptr)?(1):(-1));
	if (change > 0) {
		int di = move.di;
		while ( (di< dashpos[move.idx].size()) && (di>=0) && ( dashpos[move.idx][di] + di) < move.newDptr) {
			dashpos[move.idx][di]++;
			di++;
		}
	} else {
		int di = move.di;
		while ( (di< dashpos[move.idx].size()) && (di>=0) && ( dashpos[move.idx][di] + di) > move.newDptr) {
			dashpos[move.idx][di]--;
			di--;
		}
	}

	//sort( dashpos[move.idx].begin()  , dashpos[move.idx].end());
	//Now that di's have been swapped
	
}

void movefromLocal( vector<string>& localsequences , vector< vector<int> >& localdashpos , int& locallength , double& localcost) {
	cost = localcost;
	length = locallength;
	sequences = localsequences;
	dashpos = localdashpos;
}


void moveToLocal( vector<string>& localsequences , vector< vector<int> >& localdashpos , int& locallength , double& localcost) {
	localcost = cost ;
	locallength = length ;
	localsequences = sequences;
	localdashpos = dashpos;
}