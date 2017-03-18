#include "cTerm.h"
#include "Constants.h"
#include "cDataBlock.h"


cTerm::cTerm(): mData(nullptr)
{
}


cTerm::cTerm(int termLen, char* term)
{
	mData = new char[ORDER_TERM + termLen]();
	SetID(id++);
	SetFrequency(frequency);
	SetTermLen(termLen);
	SetTerm(term, termLen);
}


cTerm::~cTerm()
{
}
