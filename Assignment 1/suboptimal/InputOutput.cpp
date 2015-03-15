#include <iostream>
#include <vector>
#include "formulation.cpp"
#include <limits>

#define SMALL_INT std::numeric_limits<int>::min()
#define NODE_EXPANSION_RATE 100000
using namespace std;

void input(SeqProblem& p)
{
	cin >> p.clockTime;
		p.clockTime *= 60; // In seconds.
	cin >> p.aSize;
	string s;
	p.alphabet.resize(p.aSize);
	for(int i = 0; i < p.aSize; ++i)
	{
		cin >> s;
		p.alphabet[i] = s[0];
		p.charToInt[s[0]] = i;
	}
	p.alphabet.push_back('-');
	p.charToInt['-'] = p.aSize;
	cin >> p.k;
	p.sequences.resize(p.k);
	p.stringLengths.resize(p.k);
	p.sumOfLengths = 0;
	p.minimumFinalLength = 0; //Starts at zero and works its way up. It is also the length of the longest sequence.
	p.maximumFinalLength = 0;
	for(int i = 0; i < p.k; ++i)
	{
		cin >> s;
		p.sequences[i] = s;
		p.stringLengths[i] = s.size();
		p.maximumFinalLength += s.size();
		p.sumOfLengths += s.size();
		if ( p.minimumFinalLength < s.size() ) {
			p.minimumFinalLength = s.size();
		}
	}
	cin >> p.CC;
	p.MC.resize(p.aSize+1);
	for(int i = 0; i < p.aSize + 1; ++i)
	{
		//AKSHAY STYLE :P int y = p.charToInt[alphabet[i]];
		p.MC[i].resize(p.aSize+1);
		for(int j = 0; j < p.aSize + 1; ++j)
		{
			// AKSHAY STYLE :P int z = (int) p.alphabet[j];
			cin >> p.MC[i][j];
		}
	}
	cin >> s;
	p.longerLengthChildren = p.minimumFinalLength / 4;
	p.shorterLengthChildren = p.minimumFinalLength / 4;
	p.TIME_BUFFER = 4.0*pow( (p.minimumFinalLength*p.k) , 2)/NODE_EXPANSION_RATE;
	cout << "TIME BUFFER is : " << p.TIME_BUFFER << "\n";
} //Works.

void output(SeqProblem& p , SeqState& n)
{
	p.printState(n);
} //Should work.