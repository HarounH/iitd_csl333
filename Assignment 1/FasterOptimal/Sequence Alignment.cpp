#ifndef SEQ_H
#define SEQ_H
#include <iostream>
#include <vector>

using namespace std;

class SeqNode {
public:
	double currentCost;
	vector<int> position;
	double heuristicCost;
	double totalCost;
	SeqNode *parent;
	SeqNode *child;
	bool explored;
	bool isGoal;
	
	SeqNode()
	{
		parent = NULL;
		child = NULL;
	}

	SeqNode(const SeqNode& s)
	{
		currentCost = s.currentCost;
		position.resize((int) s.position.size());
		copy(s.position.begin(), s.position.end(), position.begin());
		heuristicCost = s.heuristicCost;
		totalCost = s.totalCost;
		parent = s.parent;
	}
	
	SeqNode& operator=(const SeqNode& s)
	{
		currentCost = s.currentCost;
		position.resize((int) s.position.size());
		copy(s.position.begin(), s.position.end(), position.begin());
		heuristicCost = s.heuristicCost;
		totalCost = s.totalCost;
		parent = s.parent;
		return *this;
	}
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
	vector<vector<vector<vector<double> > > > A; //lookup table
	SeqNode initialState;
	void getSuccessors(SeqNode* n, vector<SeqNode*>& successors);
	bool goalTest(SeqNode* n);
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
	double dpHeuristicFunc(SeqProblem& p, SeqNode* n)
	{
		double cost = 0;
		int max = 0, maxIndex = 0;
		for(int i = 0; i < p.k; i++)
		{
			for(int j = i + 1; j < p.k; j++)
				cost += p.A[i][j-i-1][n->position[i]][n->position[j]];
		}
		return cost + dashHeuristicFunc(p, n);			
	}
	/*
	//
	Very naive heuristic.
	//
	*/
	double dashHeuristicFunc(SeqProblem& p, SeqNode* n)
	{
		double cost = 0;
		int max = 0, maxIndex = 0;
		for(int i = 0; i < p.k; i++)
		{
			if((p.stringLengths[i] - n->position[i]) > max)
			{
				max = p.stringLengths[i] - n->position[i];
				maxIndex = i;
			}
		}
		for(int i = 0; i < p.k; i++)
				cost += p.CC * (max - (p.stringLengths[i] - n->position[i]));
		return cost;
	}
};

bool SeqProblem::goalTest(SeqNode *n)
{
	for (int i = 0; i < stringLengths.size(); i++)
	{
		if (n->position[i] != stringLengths[i])
			return false;
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
			A[i][ind][stringLengths[i]][stringLengths[j]] = 0;
			for(int l = stringLengths[i] - 1; l >= 0; l--)
			{
				A[i][ind][l].resize(stringLengths[j] + 1);
				A[i][ind][l][stringLengths[j]] = A[i][ind][l+1][stringLengths[j]] + MC[(int) sequences[i][l]][(int)'-'] + CCmin;
			}
			for(int l = stringLengths[j] - 1; l >= 0; l--)
			{
				A[i][ind][stringLengths[i]][l] = A[i][ind][stringLengths[i]][l+1] + MC[(int)'-'][(int) sequences[j][l]] + CCmin;
			}
			for(int l = stringLengths[i] - 1; l >= 0; l--)
			{
				for(int m = stringLengths[j] - 1; m >= 0; m--)
				{
					int x = A[i][ind][l+1][m+1] + MC[(int) sequences[i][l]][(int) sequences[j][m]];
					int y = A[i][ind][l+1][m] + MC[(int) sequences[i][l]][(int)'-'] + CCmin;
					int z = A[i][ind][l][m+1] + MC[(int)'-'][(int) sequences[j][m]] + CCmin;
					int less = min(min(x, y), z);
					A[i][ind][l][m] = less;
				}
			}
		}
	}
}

/*
This has quite a bit of copying.
To be improved.
*/
void SeqProblem::getSuccessors(SeqNode *n, vector<SeqNode*>& successors)
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
		SeqNode *s = new SeqNode;
		s->currentCost = 0;
		s->position.resize(k);
		vector<int> tuple(k, 0);
		invalid = false;
		int j = i;
		int tupleIndex = 0;
		copy(n->position.begin(), n->position.end(), s->position.begin());
		while (j != 0)
		{
			int bit = j & 1;
			s->position[tupleIndex] = n->position[tupleIndex] + bit;
			tuple[tupleIndex] = bit;
			if (s->position[tupleIndex] > stringLengths[tupleIndex])
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
				s->currentCost += CC;
				// if(s.auxData[y].size() > sumOfLengths)
// 					invalid = true;
			}
			for(int z = y + 1; z < k; z++)
			{
				if(tuple[y] == 0 && tuple[z] == 0);
				else if(tuple[y] == 0)
					s->currentCost += MC[(int) '-'][(int) sequences[z][s->position[z] - 1]];
				else if(tuple[z] == 0)
					s->currentCost += MC[(int) sequences[y][s->position[y] - 1]][(int) '-'];
				else
					s->currentCost += MC[(int) sequences[y][s->position[y] - 1]][(int) sequences[z][s->position[z] - 1]];
			}
		}
		if(invalid)
			continue;
		s->currentCost += n->currentCost;
		SearchAgent sa;
		s->heuristicCost = sa.dpHeuristicFunc(*this, s);
		s->totalCost = s->currentCost + s->heuristicCost;
		s->parent = n;
		s->explored = false;
		s->isGoal = false;
		successors.push_back(s);
	}
}

#endif