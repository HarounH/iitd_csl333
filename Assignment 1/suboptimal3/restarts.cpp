#include <algorithm>
void randomRestart();
void lengthAveragedRestart();
void evalDelta_l();

void restart( MODE mode = RESTART_MEANLENGTH ) {
	if ( mode == RESTART_RANDOM ) {
		randomRestart();
	} else if ( mode == RESTART_MEANLENGTH ) {
		lengthAveragedRestart();
	}
	cost = evalCost();
}

void randomRestart() {
	//length init
	//uniform_int_distribution<int> linit(minFinalLength , 2.5*minFinalLength);//, minFinalLength + ((maxFinalLength-minFinalLength)));
	length = minFinalLength + (rand()%((int)(1 + (1.5*minFinalLength))));
	//dash init
	//uniform_int_distribution<int> dinit( 0 , length);
	for(int idx = 0; idx<k; ++idx) {
		for(int didx = dashpos[idx].size()-1 ; didx >= 0 ; --didx) {
			sequences[idx].erase( sequences[idx].begin() + ( dashpos[idx][didx]+didx) ); 
		}
		dashpos[idx].resize(0);
		for(int didx = 0; didx < (length - seqLengths[idx]) ; ++didx) {
			dashpos[idx].push_back( (rand()%(seqLengths[idx] + 1)) ); // Because the valuesa at dashpos[idx][didx] represent how many chracters before the didx'th dash.
		}
		sort(dashpos[idx].begin() , dashpos[idx].end());
		for(int didx = 0; didx < dashpos[idx].size() ; ++didx) {
			sequences[idx].insert( sequences[idx].begin() + (didx + dashpos[idx][didx]), 1, '-' ); //Insert dashes in the appropriate locations
		}
	}
}

int evalDelta_l( MODE mode = SCHEDULE_RANDOM) {
	int dl = 0;
	if(mode == SCHEDULE_RANDOM) {
		dl = length - minFinalLength;
	} else if ( mode == SCHEDULE_BYRESTARTS ) {
		//needs to be inversely proportional to nrestarts
		dl = (length * (( (int)( (double)(maxFinalLength-minFinalLength)/(double)nrestarts) ) % (maxFinalLength - length) )) ;
	} 
	return dl;
}
void lengthAveragedRestart() {
	int delta_l = min( length - minFinalLength , evalDelta_l( SCHEDULE_BYRESTARTS) );
	//uniform_int_distribution<int> linit( length - delta_l , length + delta_l );
	length = length - delta_l + (rand()%((int)( 1 + 2*delta_l)));
	//dash init
	for(int idx = 0; idx<k; ++idx) {
		for(int didx = dashpos[idx].size()-1 ; didx >= 0 ; --didx) {
			sequences[idx].erase( sequences[idx].begin() + ( dashpos[idx][didx]+didx) ); 
		}
		//Assert : All the strings are back to normal. - CHECKED.
		dashpos[idx].resize(0);
		for(int didx = 0; didx < length - seqLengths[idx]; ++didx) {
			dashpos[idx].push_back( (rand()%(seqLengths[idx] + 1)) ); // Because the valuesa at dashpos[idx][didx] represent how many chracters before the didx'th dash.
		}
		sort(dashpos[idx].begin() , dashpos[idx].end());
		for(int didx = 0; didx< dashpos[idx].size() ; ++didx) {
			sequences[idx].insert( sequences[idx].begin() +  (didx + dashpos[idx][didx]) , 1, '-' ); //Insert dashes in the appropriate locations
		}
	}	
}

