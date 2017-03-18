#pragma once
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Constants.h"

class cTerm
{

private:
	char* mData;

	int ORDER_ID = 0;
	int ORDER_FREQUENCY = 4;
	int ORDER_TERMLEN = 8;
	int ORDER_TERM = 9;

	int SIZE_ID = 4;
	int SIZE_FREQUENCY = 4;
	int SIZE_TERMLEN = 1;
public:
	cTerm();
	cTerm(int termLen, char * term);
	~cTerm();


	inline void SetID(unsigned int id) const
	{
		*((unsigned int*)(mData + ORDER_ID)) = id;
	}

	inline unsigned int GetID() const
	{
		return *reinterpret_cast<unsigned int*>(mData + ORDER_ID);
	}

	inline void SetFrequency(unsigned int frequency) const
	{
		*((unsigned int*)(mData + ORDER_FREQUENCY)) = frequency;
	}

	inline void SetTermLen(int termLen) const
	{
		*(mData + ORDER_TERMLEN) = static_cast<char>(termLen);
	}

	inline void SetTerm(char * term, int termLen) const
	{
		memcpy(mData + ORDER_TERM, term, termLen);
	}

	inline char * cTerm::GetData() const
	{
		return mData;
	}

	inline unsigned int GetFrequency() const
	{
		return *reinterpret_cast<unsigned int*>(mData + ORDER_FREQUENCY);
	}

	inline char GetTermLen() const
	{
		return *(mData + ORDER_TERMLEN);
	}

	inline unsigned char * GetTerm(int termLen) const
	{
		auto tempTerm = new unsigned char[9 + termLen]();

		memcpy(tempTerm, mData + ORDER_TERM, 9 + termLen);

		return tempTerm;
	}
	
	void cTerm::Print() const
	{
		int termlen = static_cast<int>(GetTermLen(GetData()));

		int id = GetID();
		printf("Id:%d\n", GetID());
		printf("Frequency:%d\n", GetFrequency());
		printf("TermLen:%1d\n", termlen);
		printf("Term:%.*s\n", termlen, GetTerm(termlen));
		printf("Hash:%d\n", Hash(this->GetData()));
		std::cout << std::endl;
	}

	static unsigned int Hash(const char * item)
	{
		int termLen = static_cast<int>(GetTermLen(item));
		/*char * str = new char[termLen + 1];

		memcpy(str, item + 9, termLen);
		str[termLen] = '\0';

		unsigned int hashValue = (*reinterpret_cast<unsigned int*>(str) >> termLen);
		delete[] str;
		return hashValue;*/

		int hash = 0;
		for(int i =0; i<termLen; i++)
		{
			if (i == 0)
				hash += static_cast<unsigned int>(item[i + 9]);
			else
			{
				hash += i * static_cast<unsigned int>(item[i + 9]);
			}
		}
		return hash;

	}

	static char GetTermLen(const char* item)
	{
		return *(item + 8);
	}

	static unsigned int GetSize(const char * item)
	{
		int termLen = static_cast<int>(GetTermLen(item));
		return 9 + termLen;
	}

	static bool Compare(const char* mdata, const char * item)
	{
		int ORDER_TERMLEN = 8;
		int ORDER_TERM = 9;

		if (*((unsigned char*)(mdata + ORDER_TERMLEN)) != *((unsigned char*)(item + ORDER_TERMLEN)))
			return false;

		int termLen = static_cast<int>(GetTermLen(item));

		for (int i = 0; i < termLen; i++)
		{
			if (mdata[i + ORDER_TERM] != item[i + ORDER_TERM])
			{
				//nonInserted++;
				return false;
			}
		}

		return true;
	}

};

