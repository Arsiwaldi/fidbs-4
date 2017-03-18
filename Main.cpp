#pragma warning(disable:4996)
#pragma once

#include <chrono>
#include <ctime>
#include "cTerm.h"
#include<iostream>
#include "cDataBlock.h"
#include "HashTable.h"
#include <string>
#include "cLineReader.h"
#include "cTermIndex.h"
#include "ArrayTester.h"
#include <chrono>
#include <thread>
#include "Constants.h"

using namespace std;
using namespace chrono;
typedef cTermIndex<char *> TTermIndex;


static uint AddTerm(TTermIndex * termIndex, char * term, uint len)
{
	vectorTester.AddTerm(term,len);
	cTerm * termData = new cTerm(len, term);
	termIndex->Insert(termData->GetData());
	delete termData;

	return 0;
}

static bool FindTerm(TTermIndex * termIndex, char * term, uint len)
{
	cTerm * termData = new cTerm(len, term);
	return termIndex->Find(termData->GetData());
}

static void AddNGram(TTermIndex * termIndex, char * line, uint lineLen)
{
	char *pterm = line;
	uint IonIndex = 0;
	const uint MAX_NGRAM_LEN = 1024;
	uint ngram[MAX_NGRAM_LEN];
	uint ngramLen = 0;

	for (uint i = 0; i < lineLen; i++)
	{
		if (line[i] == ' ')
		{
			uint termId = AddTerm(termIndex, pterm, i - IonIndex);
			ngram[ngramLen++] = termId;

			IonIndex = i + 1;
			pterm = line + IonIndex;
		}
	}

	uint termId = AddTerm(termIndex, pterm, lineLen - IonIndex);
	ngram[ngramLen++] = termId;
}

static void FindNGram(TTermIndex * termIndex, char * line, uint lineLen)
{
	char *pterm = line;
	uint IonIndex = 0;
	const uint MAX_NGRAM_LEN = 1024;
	uint ngram[MAX_NGRAM_LEN];
	uint ngramLen = 0;

	for (uint i = 0; i < lineLen; i++)
	{
		if (line[i] == ' ')
		{
			uint termId = FindTerm(termIndex, pterm, i - IonIndex);
			ngram[ngramLen++] = termId;

			IonIndex = i + 1;
			pterm = line + IonIndex;
		}
	}

	uint termId = FindTerm(termIndex, pterm, lineLen - IonIndex);
	ngram[ngramLen++] = termId;
}

static void LoadInitFile(TTermIndex * termIndex, cLineReader * lineReader)
{
	char * line;
	uint lineLen;
	while ((line = lineReader->GetLine(lineLen)) != NULL)
	{
		if (strcmp(line, "S") == 0)
		{
			break;
		}

		AddNGram(termIndex, line, lineLen);
	}
}

static void LoadWorkLoad(TTermIndex * termIndex, cLineReader * lineReader)
{
	uint queryCount = 0, insertCount = 0, deleteCount = 0, lineLen;
	char op;
	char* line;

	while ((line = lineReader->GetLine(lineLen)) != NULL)
	{
		
		sscanf(line, "%c", &op);
		char * str = line + 2;
		uint strLen = lineLen - 2;

		if( op == 'A')
		{
			AddNGram(termIndex, line, strLen);
			insertCount++;
		}

		else if (op == 'D')
		{
			deleteCount++;
		}

		else if(op == 'Q')
		{
			FindNGram(termIndex, line, lineLen);
			queryCount++;
		}
	}
}

int main() {


#pragma endregion

	int initCount = 0;
	int workloadCount = 0;

	int initVectorCount = 0;
	int workloadVectorCount = 0;

	TTermIndex * termindex = new TTermIndex(40, 5000);
	cLineReader * lineReader = new cLineReader("testingData", nullptr);
	
	
	cout << "###### ---- HASH TABLE ---- ######" << endl << endl;

	auto t1 = high_resolution_clock::now();
	//LoadInit
	LoadInitFile(termindex, lineReader);
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> procTime = t2 - t1;
	cout << "INIT PART - PROCESSING TIME:" << (procTime.count() / 1000.0) << "sec" << endl;
	cout << "Count of inserted items after init: " << termindex->mCount << endl << endl;
	initCount = termindex->mCount;
	initVectorCount = vectorTester.terms.size();

	auto t3 = high_resolution_clock::now();
	//LoadWorkload
	LoadWorkLoad(termindex, lineReader);
	auto t4 = high_resolution_clock::now();
	duration<double, std::milli> procTime2 = t4 - t3;
	cout << "LOADWORKLOAD - PROCESSING TIME:" << (procTime2.count() / 1000.0) << "sec" << endl;
	cout << "Count of inserted items after workload: " << termindex->mCount - initCount << endl <<endl;
	workloadCount = termindex->mCount;

	cout << "STATISTICS:" << endl;
	termindex->GetStats();
	cout << endl;

	cout << "###### ---- VECTOR<string> FOR TESTING INSERTED ITEMS ---- ######" << endl << endl;
	cout << "After init: " << initVectorCount << endl;
	cout << "After workload:" <<  vectorTester.terms.size() - initVectorCount << endl;
	cout << "Summary:" << vectorTester.terms.size() << endl;

	return 0;
}