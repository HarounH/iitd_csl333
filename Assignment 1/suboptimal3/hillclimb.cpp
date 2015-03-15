#ifndef SUB_OPT_SEARCH_CPP
	#define SUB_OPT_SEARCH_CPP

#include <limits>
#define BIG_DOUBLE std::numeric_limits<double>::max()

void hilldescent_norestarts_untimed(int MAX_PLATEAU_MOVES = 0) {
	Move bestmove;
		bestmove.newcost = BIG_DOUBLE;
	int plateauctr = 0;
	while(true) { //Breaks on the first optima after looking to escape plateaus
		vector<Move> moves;
		Move nxtbstmov;
			nxtbstmov.newcost = BIG_DOUBLE;
		getMoves(moves);
		for(int i=0; i<moves.size(); ++i) {
			if (moves[i].newcost < nxtbstmov.newcost ) {
					nxtbstmov.idx = moves[i].idx;
					nxtbstmov.di = moves[i].di;
					nxtbstmov.origDptr = moves[i].origDptr;
					nxtbstmov.newDptr = moves[i].newDptr;
					nxtbstmov.newcost = moves[i].newcost;
			}
		}
		if ( nxtbstmov.newcost < bestmove.newcost ) {
			bestmove = nxtbstmov;
			plateauctr = 0;
			setState(bestmove);
		} else if (nxtbstmov.newcost == bestmove.newcost && (plateauctr < MAX_PLATEAU_MOVES)) {
			plateauctr++;
			bestmove = nxtbstmov;
			setState(bestmove);
		} else {
			break;
		}
	}
}

void hilldescent_restarts_untimed(int MAX_RESTARTS = 1 , int MAX_PLATEAU_MOVES = 0) {
	//local version stores the best so far.
	vector<string> localstrings = sequences;
	double localcost = cost;
	int locallength = length;
	vector< vector<int> > localdashpos = dashpos;
	
	while( nrestarts <= MAX_RESTARTS ) {
		int plateauctr = 0;
		//cout << "restarted with cost " << cost << " and local cost = " << localcost << "\n";
		while(true) { //Breaks on the first optima after looking to escape plateaus
			vector<Move> moves;
			Move nxtbstmov;
				nxtbstmov.newcost = BIG_DOUBLE ;
			getMoves(moves);
			for(int i=0; i<moves.size(); ++i) { //Find the best move.
				
				if (moves[i].newcost < nxtbstmov.newcost ) {
					nxtbstmov.idx = moves[i].idx;
					nxtbstmov.di = moves[i].di;
					nxtbstmov.origDptr = moves[i].origDptr;
					nxtbstmov.newDptr = moves[i].newDptr;
					nxtbstmov.newcost = moves[i].newcost;
				}
			}
			if ( nxtbstmov.newcost < cost ) {
				plateauctr = 0;
				setState(nxtbstmov);
				if ( cost < localcost) {
					moveToLocal(localstrings , localdashpos , locallength , localcost);
				}

			} else if ( (nxtbstmov.newcost == cost) && (plateauctr < MAX_PLATEAU_MOVES)) {
				plateauctr++;
				setState(nxtbstmov);
				if ( cost < localcost) {
					moveToLocal(localstrings , localdashpos , locallength , localcost);
				}
			} else {
				break;
			}
		}
		restart();
		++nrestarts;
	} //Break out of inner loop.
	if (cost > localcost) {
		movefromLocal(localstrings , localdashpos , locallength , localcost);
	}
	return;
}


void hilldescent_infRestarts_timed( clock_t& start , int MAX_PLATEAU_MOVES = 0) {
	clock_t present;
	present = clock();
	//local version stores the best so far.
	vector<string> localstrings = sequences;
	double localcost = cost;
	int locallength = length;
	vector< vector<int> > localdashpos = dashpos;
	
	while( ((float)present/CLOCKS_PER_SEC) + TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + timeLimit ) {
		int plateauctr = 0;
		//cout << "restarted with cost " << cost << " and local cost = " << localcost << "\n";
		while(true && ((float)present/CLOCKS_PER_SEC) + TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + timeLimit) { //Breaks on the first optima after looking to escape plateaus
			vector<Move> moves;
			Move nxtbstmov;
				nxtbstmov.newcost = BIG_DOUBLE ;
			getMoves(moves);
			for(int i=0; i<moves.size(); ++i) { //Find the best move.
				
				if (moves[i].newcost < nxtbstmov.newcost ) {
					nxtbstmov.idx = moves[i].idx;
					nxtbstmov.di = moves[i].di;
					nxtbstmov.origDptr = moves[i].origDptr;
					nxtbstmov.newDptr = moves[i].newDptr;
					nxtbstmov.newcost = moves[i].newcost;
				}
			}
			if ( nxtbstmov.newcost < cost ) {
				plateauctr = 0;
				setState(nxtbstmov);
				if ( cost < localcost) {
					moveToLocal(localstrings , localdashpos , locallength , localcost);
				}

			} else if ( (nxtbstmov.newcost == cost) && (plateauctr < MAX_PLATEAU_MOVES)) {
				plateauctr++;
				setState(nxtbstmov);
				if ( cost < localcost) {
					moveToLocal(localstrings , localdashpos , locallength , localcost);
				}
			} else {
				break;
			}
			present = clock();
		}
		if((float)present/CLOCKS_PER_SEC) + TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + timeLimit)
			restart( RESTART_RANDOM );
		else
		    break;
		++nrestarts;
	} //Break out of inner loop.
	if (cost > localcost) {
		movefromLocal(localstrings , localdashpos , locallength , localcost);
	}
	return;
}

#endif