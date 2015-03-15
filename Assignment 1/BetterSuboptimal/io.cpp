#ifndef IO_CPP
	#define IO_CPP
#include <iostream>
#include <vector>
#include "formulation.cpp"
#include <limits>

#define SMALL_INT std::numeric_limits<int>::min()
#define NODE_EXPANSION_RATE 100000
using namespace std;

void input(Problem& p)
{
	cin >> p.timeLimit;
		p.timeLimit *= 60; // In seconds.
	cin >> p.aSize;
	string s;
	p.idxToChar.resize(p.aSize);
	for(int i = 0; i < p.aSize; ++i)
	{
		cin >> s;
		p.idxToChar[i] = s[0];
		p.charToIdx[s[0]] = i;
	}
	p.idxToChar.push_back('-');
	p.charToIdx['-'] = p.aSize;
	cin >> p.k;
	p.inSequences.resize(p.k);
	p.seqLengths.resize(p.k);
	p.minFinalLength = 0; //Starts at zero and works its way up. It is also the length of the longest sequence.
	p.maxFinalLength = 0;

	for(int i = 0; i < p.k; ++i)
	{
		cin >> s;
		p.inSequences[i] = s;
		p.seqLengths[i] = s.size();
		p.maxFinalLength += s.size();
		if ( p.minFinalLength < s.size() ) {
			p.minFinalLength = s.size();
		}
	}
	cin >> p.cc;
	
	p.mc.resize(p.aSize+1);
	
	for(int i = 0; i < p.aSize + 1; ++i)
	{
		//AKSHAY STYLE :P int y = p.charToInt[alphabet[i]];
		p.mc[i].resize(p.aSize+1);
		for(int j = 0; j < p.aSize + 1; ++j)
		{
			// AKSHAY STYLE :P int z = (int) p.alphabet[j];
			cin >> p.mc[i][j];
		}
	}
	cin >> s;
	p.TIME_BUFFER = 4.0*pow( (p.minFinalLength*p.k) , 2)/NODE_EXPANSION_RATE;
} //Works.

void output(Problem& p , State& n)
{
	p.printState(n);
} //Should work.

#endif