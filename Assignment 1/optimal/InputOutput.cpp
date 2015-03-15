#include <iostream>
#include <vector>
#include "Sequence Alignment.cpp"

using namespace std;

void input(SeqProblem& p)
{
	cin >> p.clockTime;
	cin >> p.aSize;
	string s;
	p.alphabet.resize(p.aSize);
	for(int i = 0; i < p.aSize; ++i)
	{
		cin >> s;
		p.alphabet[i] = s[0];
	}
	p.alphabet.push_back('-');
	cin >> p.k;
	p.sequences.resize(p.k);
	p.stringLengths.resize(p.k);
	p.sumOfLengths = 0;
	for(int i = 0; i < p.k; ++i)
	{
		cin >> s;
		p.sequences[i] = s;
		p.stringLengths[i] = s.size();
		p.sumOfLengths += s.size();
	}
	cin >> p.CC;
	p.MC.resize(256);
	for(int i = 0; i < p.aSize + 1; ++i)
	{
		int y = (int) p.alphabet[i];
		if(p.MC[y].empty())
			p.MC[y].resize(256);
		for(int j = 0; j < p.aSize + 1; ++j)
		{
			int z = (int) p.alphabet[j];
			cin >> p.MC[y][z];
		}
	}
	cin >> s;
}

void output(SeqNode& n)
{
	for(int i = 0; i < n.auxData.size(); ++i)
		cout<<n.auxData[i]<<"\n";
}