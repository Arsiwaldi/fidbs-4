#pragma once
#include <vector>
#include <algorithm>
using namespace std;
class ArrayTester
{
public:
	vector<string>terms;
	ArrayTester()
	{};
	~ArrayTester() {};


	inline void AddTerm(char * term, int termlen)
	{
		char * buffer = new char[termlen + 1]();
		memcpy(buffer, term, termlen);
		buffer[termlen] = '\0';
		auto it = terms.begin();
		string str = buffer;
		delete []buffer;

		if(FindTerm(str) == false)
		{
			terms.push_back(str);
		}
		
	}

	inline bool FindTerm (string term)
	{
		if (std::find(terms.begin(),terms.end(), term) != terms.end()) {
			return true;
		}
		else
		{
			return false;
		}
	}
};


