#ifndef SUB_OPT_SEARCH_CPP
	#define SUB_OPT_SEARCH_CPP

#include <random> //...for simulated annealing.

#define BIG_DOUBLE std::numeric_limits<double>::max()

//enum INIT_TYPE { RANDOM = 0 , GENETIC = 1 };


/*
Does not handle shoulders, pits, plateaus or ANY of that.
*/
State greedyHillClimb_NoRestarts_untimed(Problem& p) {
	State best = p.initState; //Copy cost.
	while(true) {
		vector<State> nbd;
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
		if( best.cost <= minCost ) {
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
State greedyHillClimb_nRestarts_untimed(Problem& p , int n) {
	State best = p.initState;
	State prevBest = p.initState;
	int restarts = n;
	while( --restarts ) {
		cout << "restart # = " << restarts << " with best cost = " << prevBest.cost << "\n";
		while(true) {
			cout << "best cost = " << best.cost << "\n";
			vector<State> nbd;
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
			if( best.cost <= minCost ) { //Reach a plateau and break.
				break;
			} 
			else if (best.cost > minCost) {
				best = nbd[minIDX]; //New best.
			}
		}
		if(prevBest.cost > best.cost) {
			prevBest = best;
		}
		p.randomState(best , prevBest); //Generate 1 Random Start. ALTER THIS for better greedyhillClimb.
	}
	return (( best.cost < prevBest.cost )?( best ):( prevBest ));
}

/*
Need some way of evaluation TIME_BUFFER. It allows for the program to print stuff etc.
*/

/*
Greedy Hill Climb with no restarts but it is timed. Theoretically runs into shoulders etc and treats it as optimal.
*/
State greedyHillClimb_NoRestarts_timed(Problem& p , clock_t& start) {
	clock_t present;
	present = clock();
	State best  = p.initState;
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.timeLimit) {
		cout << p.timeLimit;
		vector<State> nbd;
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
State greedyHillClimb_infRestarts_timed(Problem& p , clock_t& start) {
	clock_t present;
	present = clock();
	State best = p.initState;		//...Extra copy.
	State prevBest = p.initState; //...Extra copy.
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.timeLimit) {
		cout << "restart " << prevBest.cost << "\n";
		while(true && ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.timeLimit ) { //You don't want to get caught in this.
			vector<State> nbd;
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
		p.randomState(best , prevBest);
	}
	cout << (((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER - ((float)start/CLOCKS_PER_SEC) - p.timeLimit) << "timed \n";
	return (( best.cost < prevBest.cost )?( best ):( prevBest ));
}


State greedyHillClimb_limitedSteps_nRestarts_untimed(Problem& p , int MAX_STEPS , int restarts ) {
	clock_t present;
	present = clock();
	State best = p.initState;
	State prevBest = p.initState;
	while( restarts-- ) {
		cout << " restart # " <<  restarts << " best cost = " << prevBest.cost << "\n";
		int stepCount = 0;
		while( (stepCount < MAX_STEPS)) {
			cout << "best cost = " << best.cost << "\n";
			vector<State> nbd; //This loop needs to take less than the time buffer.
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
		p.randomState(best , prevBest); //Should generate varying lengths too.
	}
}
State greedyHillClimb_limitedSteps_infRestarts_timed(Problem& p ,clock_t& start, int MAX_STEPS ) {
	clock_t present;
	present = clock();
	State best = p.initState;
	State prevBest = p.initState;
	int restarts=0;
	while( ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.timeLimit) {
		restarts++;
		int stepCount = 0;
		while( (stepCount < MAX_STEPS) && ((float)present/CLOCKS_PER_SEC) + p.TIME_BUFFER < ((float)start/CLOCKS_PER_SEC) + p.timeLimit) {
			vector<State> nbd; //This loop needs to take less than the time buffer.
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
		p.randomState(best , prevBest); //Should generate varying lengths too.
	}
}
#endif