#ifndef SUB_OPT_SEARCH_CPP
	#define SUB_OPT_SEARCH_CPP

#include <random> //...for simulated annealing.

#define BIG_DOUBLE std::numeric_limits<double>::max()

//enum INIT_TYPE { RANDOM = 0 , GENETIC = 1 };


/*
Does not handle shoulders, pits, plateaus or ANY of that.
*/
SeqState greedyHillClimb_NoRestarts_untimed(SeqProblem& p) {
	SeqState best = p.initialState; //Copy cost.
	while(true) {
		vector<SeqState> nbd;
		p.getNBD(best , nbd);
		//Find minimum of the neigbhours.
		int minIDX = 0;
		double minCost = BIG_DOUBLE; //Defined as numeric limit.
		for(int i = 0; i<nbd.size() ; ++i) {
			if ( nbd[i].cost < 0) p.printState(nbd[i]);
			if (nbd[i].cost < minCost) {
				minCost = nbd[i].cost;
				minIDX = i;
			}
		} //Minimum found at i.
		if( best.cost < minCost ) {
			break;
		} else {
			best = nbd[minIDX]; //New best.
		}
	}
	return best;
}

/*
Restarts but does not time itself.
*/
SeqState greedyHillClimb_nRestarts_untimed(SeqProblem& p , int n) {
	SeqState best = p.initialState;
	SeqState prevBest = p.initialState;
	int restarts = n;
	while( --restarts ) {
		int sameCostMoves = 0;
		while(true) {
			vector<SeqState> nbd;
			p.getNBD(best , nbd);
			//Find minimum of the neigbhours.
			int minIDX = 0;
			double minCost = BIG_DOUBLE; //Defined as numeric limit.
			for(int i = 0; i<nbd.size();++i) {
				if (nbd[i].cost < minCost) {
					minCost = nbd[i].cost;
					minIDX = i;
				}
			} //Minimum found at i.
			if( best.cost < minCost ) {
				break;
			} 
			else if (best.cost > minCost) {
				best = nbd[minIDX]; //New best.
				//Compare the previous bests too.
				sameCostMoves = 0;
			}
			else {
				if(sameCostMoves > -1)
					break;
				++sameCostMoves;
				best = nbd[minIDX];
			}
		}
		if(prevBest.cost > best.cost)
		{
			prevBest = best;
		}
		cout<<restarts<<"\n";
		p.initializeInto(STATS , best , prevBest); //Generate 1 Random Start. ALTER THIS for better greedyhillClimb.

	}
	return (( best.cost < prevBest.cost )?( best ):( prevBest ));
}

/*
Need some way of evaluation TIME_BUFFER. It allows for the program to print stuff etc.
*/

/*
Greedy Hill Climb with no restarts but it is timed. Theoretically runs into shoulders etc and treats it as optimal.
*/
SeqState greedyHillClimb_NoRestarts_timed(SeqProblem& p , clock_t& start) {
	clock_t present;
	present = clock();
	SeqState best  = p.initialState;
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.clockTime) {
		cout << p.clockTime;
		vector<SeqState> nbd;
		p.getNBD(best , nbd);
		//Find minimum of the neigbhours.
		int minIDX=0;
		double minCost = BIG_DOUBLE; //Defined as numeric limit.
		for(int i = 0; i<nbd.size();++i) {
			if (nbd[i].cost < minCost) {
				minCost = nbd[i].cost;
				minIDX = i;
			}
		} //Minimum found at i.
		if( best.cost < minCost )
			break;
		else
			best = nbd[minIDX]; //New best.
		present = clock(); //Get time.
	}
	return best;
}

/*
Greedy Hill Climb with infinite restarts if time permits.
*/
SeqState greedyHillClimb_infRestarts_timed(SeqProblem& p , clock_t& start , INIT_TYPE mode = STATS) {
	clock_t present;
	present = clock();
	SeqState best = p.initialState;		//...Extra copy.
	SeqState prevBest = p.initialState; //...Extra copy.
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.clockTime) {
		cout << "restart " << prevBest.cost << "\n";
		while(true && ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.clockTime ) { //You don't want to get caught in this.
			vector<SeqState> nbd;
			p.getNBD(best , nbd);
			//Find minimum of the neigbhours.
			int minIDX=0;
			double minCost = BIG_DOUBLE; //Defined as numeric limit.
			for(int i = 0; i<nbd.size();++i) {
				if (nbd[i].cost < minCost) {
					minCost = nbd[i].cost;
					minIDX = i;
				}
			} //Minimum found at i.
			if( best.cost <= minCost ){
				break;
			} else {
				best = nbd[minIDX]; //New best.
				
			}
			present = clock();
		}
		if (prevBest.cost > best.cost) 
			prevBest = best;
		p.initializeInto(mode , best , prevBest);
	}
	cout << (((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER - ((float)start/CLOCKS_PER_SEC) - p.clockTime) << "timed \n";
	return (( best.cost < prevBest.cost )?( best ):( prevBest ));
}

#define MAX_STEPS 10
SeqState greedyHillClimb_limitedSteps_infRestarts_timed(SeqProblem& p ,clock_t& start , INIT_TYPE mode = STATS) {
	clock_t present;
	present = clock();
	SeqState best = p.initialState;
	SeqState prevBest = p.initialState;
	int restarts=0;
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.clockTime) {
		restarts++;
		int stepCount = 0;
		while( (stepCount < MAX_STEPS) && ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.clockTime) {
			vector<SeqState> nbd; //This loop needs to take less than the time buffer.
			p.getNBD(best , nbd);
			int minIDX = 0;
			double minCost = BIG_DOUBLE;
			for(int i =0; i<nbd.size(); ++i) {
				if (nbd[i].cost < minCost) {
					minCost = nbd[i].cost;
					minIDX = i;
				}
			}
			if (best.cost <= minCost) {
				break;
			} else {
				best = nbd[minIDX];
			}
			++stepCount;
			present = clock();
		}
		if (prevBest.cost >= best.cost) {
			prevBest = best;
		}
		p.initializeInto(mode , best , prevBest); //Should generate varying lengths too.
	}
}
#endif