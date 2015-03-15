double evalCost() {
	double ans=0.0;
	for(int idx1 = 0; idx1 < k ; ++idx1) {
		ans += cc*dashpos[idx1].size();
		for(int idx2 = idx1+1; idx2 < k ; ++idx2) {
			for(int pos=0; pos<length; ++pos) {
				ans += mc[ charToIdx[ (int) sequences[idx1][pos] ] ][ charToIdx[ (int) sequences[idx2][pos] ] ];
			}
		}
	}
	return ans;
}

double evalCost_move(Move move) {
	//Given idx, given origDidx, newDidx.
	double dcost=0.0;
	//ASSERT : No cost for cc.
	for(int sidx=0; sidx<k; ++sidx) {
		if (sidx != move.idx) {
			double newcost = mc[ charToIdx[ (int) sequences[move.idx][move.newDptr] ] ][ charToIdx[ (int) sequences[sidx][move.origDptr] ] ]
			 				+ mc[ charToIdx[ (int) sequences[move.idx][move.origDptr] ] ][ charToIdx[ (int) sequences[sidx][move.newDptr] ] ];
			double oldcost = mc[ charToIdx[ (int) sequences[move.idx][move.origDptr] ] ][ charToIdx[ (int) sequences[sidx][move.origDptr] ] ] 
							+ mc[ charToIdx[ (int) sequences[move.idx][move.newDptr] ] ][ charToIdx[ (int) sequences[sidx][move.newDptr] ] ];
			dcost += (newcost - oldcost);
		}
	}
	return dcost;
}