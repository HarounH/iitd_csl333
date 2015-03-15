#include <iostream>
#include <vector>
#include <iterator>
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
	for(int i = 0; i < p.k; ++i)
	{
		cin >> s;
		p.sequences[i] = s;
		p.stringLengths[i] = s.size();
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

void output(SeqNode* n, SeqProblem &p)
{
	vector<string> result(p.k);
	while(n->parent->parent != NULL)
	{
		n->parent->child = n;
		n = n->parent;
	}
	while(n != NULL)
	{
		for(int i = 0; i < p.k; i++)
		{
			if(n->position[i] == n->parent->position[i])
				result[i].push_back('-');
			else
				result[i].push_back(p.sequences[i][n->position[i] - 1]);
		}
		n = n->child;
	}
	copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n"));
}