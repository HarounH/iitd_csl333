#ifndef SEQ_H
#define SEQ_H
#include <iostream>
#include <vector>

using namespace std;

class SeqNode {
public:
	double currentCost;
	vector<int> position;
	vector<string> auxData;
	double heuristicCost;
	double totalCost;
};

class SeqProblem {
public:
	double clockTime;
	int aSize;
	int k;
	double CC;
	vector< vector<double> > MC;
	vector<string> sequences;
	vector<int> stringLengths;
	vector<char> alphabet;
	int sumOfLengths;
	vector<vector<vector<vector<pair<double, int> > > > > A; //lookup table
	SeqNode initialState;
	void getSuccessors(SeqNode& n, vector<SeqNode>& successors);
	bool goalTest(SeqNode& n);
	void buildHeuristicTable();
};


class SearchAgent {
public:
	/*
	//
	Equivalent to uniform cost search.
	//
	*/
	double zeroHeuristicFunc(SeqProblem& p, SeqNode& n)
	{
		return 0.0;
	}
	/*
	//
	This is the current best one!
	//
	*/
	double dpHeuristicFunc(SeqProblem& p, SeqNode& n)
	{
		double cost = 0;
		int max = 0, maxIndex = 0;
		for(int i = 0; i < p.k; i++)
		{
			for(int j = i + 1; j < p.k; j++)
				cost += p.A[i][j-i-1][n.position[i]][n.position[j]].first;// - p.CC * p.A[i][j-i-1][n.position[i]][n.position[j]].second;
		}
		//cout<<cost<<"\n";
		return cost + dashHeuristicFunc(p, n);			
	}
	/*
	//
	Very naive heuristic.
	//
	*/
	double dashHeuristicFunc(SeqProblem&p, SeqNode& n)
	{
		double cost = 0;
		int max = 0, maxIndex = 0;
		for(int i = 0; i < p.k; i++)
		{
			if(n.auxData[i].size() > max)
			{
				max = n.auxData[i].size();
				maxIndex = i;
			}
		}
		for(int i = 0; i < p.k; i++)
		{
				cost += p.CC * (n.auxData[maxIndex].size() - n.auxData[i].size());
		}
		return cost;
	}
	/*
	//
	Experimental. Not good, not consistent either.
	//
	*/
	double dashMinMatchHeuristicFunc(SeqProblem& p, SeqNode& n)
	{
		double cost = 0;
		int max = 0, maxIndex = 0;
		for(int i = 0; i < p.k; i++)
		{
			if(n.auxData[i].size() > max)
			{
				max = n.auxData[i].size();
				maxIndex = i;
			}
		}
		for(int i = 0; i < p.k; i++)
		{
				cost += p.CC * (n.auxData[maxIndex].size() - n.auxData[i].size());
				cost += (p.k - i - 1) * (p.stringLengths[i] - n.position[i]);
		}
		return cost;
	}
};

bool SeqProblem::goalTest(SeqNode& n)
{
	for (int i = 0; i < stringLengths.size(); i++)
	{
		if (n.position[i] != stringLengths[i])
			return false;
		// for (int j = i + 1; j < stringLengths.size(); j++)
// 		{
// 			if (n.position[j] != stringLengths[j])
// 				return false;
// 			if (n.auxData[i].size() != n.auxData[j].size())
// 				return false;
// 		}
	}
	return true;
}

//build the heuristic table
void SeqProblem::buildHeuristicTable()
{
	A.resize(k);
	int CCmin = 0;
	for(int i = 0; i < k; i++)
	{
		A[i].resize(k - i - 1);
		for(int j = i + 1; j < k; j++)
		{
			int ind = j - i - 1;
			A[i][ind].resize(stringLengths[i] + 1);
			A[i][ind][stringLengths[i]].resize(stringLengths[j] + 1);
			A[i][ind][stringLengths[i]][stringLengths[j]].first = 0;
			A[i][ind][stringLengths[i]][stringLengths[j]].second = 0;
			for(int l = stringLengths[i] - 1; l >= 0; l--)
			{
				A[i][ind][l].resize(stringLengths[j] + 1);
				A[i][ind][l][stringLengths[j]].first = A[i][ind][l+1][stringLengths[j]].first + MC[(int) sequences[i][l]][(int)'-'] + CCmin;
				A[i][ind][l][stringLengths[j]].second = stringLengths[i] - l;
			}
			for(int l = stringLengths[j] - 1; l >= 0; l--)
			{
				A[i][ind][stringLengths[i]][l].first = A[i][ind][stringLengths[i]][l+1].first + MC[(int)'-'][(int) sequences[j][l]] + CCmin;
				A[i][ind][stringLengths[i]][l].second = stringLengths[j] - l;
			}
			for(int l = stringLengths[i] - 1; l >= 0; l--)
			{
				for(int m = stringLengths[j] - 1; m >= 0; m--)
				{
					int x = A[i][ind][l+1][m+1].first + MC[(int) sequences[i][l]][(int) sequences[j][m]];
					int y = A[i][ind][l+1][m].first + MC[(int) sequences[i][l]][(int)'-'] + CCmin;
					int z = A[i][ind][l][m+1].first + MC[(int)'-'][(int) sequences[j][m]] + CCmin;
					int less = min(min(x, y), z);
					if(less == x)
						A[i][ind][l][m].second = A[i][ind][l+1][m+1].second;
					else if(less == y)
						A[i][ind][l][m].second = A[i][ind][l+1][m].second + 1;
					else if(less == z)
						A[i][ind][l][m].second = A[i][ind][l][m+1].second + 1;
					A[i][ind][l][m].first = less;
				}
			}
		}
	}
}

/*
This has quite a bit of copying.
To be improved.
*/
void SeqProblem::getSuccessors(SeqNode& n, vector<SeqNode>& successors)
{
	/**for each 2^k possible tuples (consisting of 0's and 1's)
	//new position is position+tuple
	//new cost is previous cost + (CC * number of ones in tuple) + (sum of all cost pairs for that position)
	//possibly update auxData
	//call heuristic function
	//update total cost
	//push into vector
	**/
	successors.reserve(1 << k);
	bool invalid = false;
	for(int i = 1; i < (1 << k); ++i)
	{
		SeqNode s;
		s.currentCost = 0;
		s.position.resize(k);
		vector<int> tuple(k, 0);
		invalid = false;
		int j = i;
		int tupleIndex = 0;
		s.auxData.resize(k);
		copy(n.auxData.begin(), n.auxData.end(), s.auxData.begin());
		copy(n.position.begin(), n.position.end(), s.position.begin());
		while (j != 0)
		{
			int bit = j & 1;
			s.position[tupleIndex] = n.position[tupleIndex] + bit;
			tuple[tupleIndex] = bit;
			if (s.position[tupleIndex] > stringLengths[tupleIndex])
			{
				invalid = true;
				break;
			}
			j >>= 1;
			tupleIndex++;
		}
		if(invalid)
			continue;
		for(int y = 0; y < k; y++)
		{
			if(tuple[y] == 0)
			{
				s.currentCost += CC;
				if(s.position[y] < stringLengths[y])
					s.auxData[y].insert(s.auxData[y].begin() + (int) s.auxData[y].size() - stringLengths[y] + s.position[y], '-');
				else
					s.auxData[y].push_back('-');
				if(s.auxData[y].size() > sumOfLengths)
					invalid = true;
			}
			for(int z = y + 1; z < k; z++)
			{
				if(tuple[y] == 0 && tuple[z] == 0);
				else if(tuple[y] == 0)
					s.currentCost += MC[(int) '-'][(int) sequences[z][s.position[z] - 1]];
				else if(tuple[z] == 0)
					s.currentCost += MC[(int) sequences[y][s.position[y] - 1]][(int) '-'];
				else
					s.currentCost += MC[(int) sequences[y][s.position[y] - 1]][(int) sequences[z][s.position[z] - 1]];
			}
		}
		if(invalid)
			continue;
		s.currentCost += n.currentCost;
		SearchAgent sa;
		s.heuristicCost = sa.dpHeuristicFunc(*this, s);
		s.totalCost = s.currentCost + s.heuristicCost;
		successors.push_back(s);
	}
}

#endif