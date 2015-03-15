#ifndef FORMULATION_H
	#define FORMULATION_H
#include <vector>
#include <unordered_map>
#include <deque>
#include <string>
#include "formulation.h"

class State {
public:
	vector< vector<int> > dashPos; //Tells you the exact location of the dashes in the sequences.
	vector< deque<char> > sequences;
	double cost;
	int length;
};

class Problem {
public:
	//Timing
	double timeLimit; //Time Limit
	double TIME_BUFFER; 
	
	//Alphabet	
	int aSize;
	vector<char> idxToChar;
	unordered_map<char , int> charToIdx;
	
	//Sequences information
	int k; //Number of sequences/
	vector<string> inSequences;
	vector<int> seqLengths;
	int minFinalLength; //Is also max(seqLengths);
	int maxFinalLength; //is also sum(seLengths)
	
	//Costs
	double cc;
	vector< vector< double > > mc;

	//Generation
	std::random_device rd;
	std::mt19937 engine;

	//States
	State initState;

	//Constructors
	Problem();

	//Functions
	void print();
	void printState(State& s);

	//Utility functions
	double evalCost(State& state);
	double evalChildCost_singleSwap(State& parent , State& child , int stringIdx , int swapIdx1 , int swapIdx2);
	//generation functions
	void init();  //Creates a random state in initState.
		void initDashes(int idx , uniform_int_distribution<int>& dashInit); //Initializes dashes positions randomly.
		void initDeques(int idx);	//Builds the deque of characters according to the dashpos vectors.
	void randomState(State& state , State& best); //Creates a random state in state;
		int generateRandomLength(State& best); //Follow a schedule here.
		void generateRandomDashPos(int idx , uniform_int_distribution<int>& dashinit , State& s);
		void generateDeque(int idx , State& s);
	void getNBD(State& parent , vector<State>& nbd);

};

Problem::Problem() : engine(rd()) {}
#endif