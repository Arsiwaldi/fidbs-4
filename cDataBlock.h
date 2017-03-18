#pragma once
#include "cTerm.h"
#include <chrono>
#include <thread>

template <class TItem> class cDataBlock
{
private:
	unsigned int mCapacity = 8192;
	char * mData;
	unsigned int mOrder;
	int countRecords = 0;

public:
	cDataBlock<TItem>* mNextBlock;

	cDataBlock()
	{
		mOrder = 0;
		mData = new char[mCapacity]();
		mData[0] = 0;
	}
	~cDataBlock()
	{
		if (mData != nullptr)
		{
			delete mData;
			mData = NULL;
		}

		if (mNextBlock != nullptr)
		{
			delete mNextBlock;
			mNextBlock = NULL;
		}
	}

	cDataBlock<TItem> * Add(const TItem &item, int &value);
	bool Find(const TItem &item);
	char* GetData();
	unsigned int Getmorder();
	int GetFreeSpace();
};

template<class TItem>
inline cDataBlock<TItem> * cDataBlock<TItem>::Add(const TItem & item, int & block)
{
	int itemSize = cTerm::GetSize(item);
	if ( (mOrder + itemSize) <= mCapacity)
	{
		memcpy(mData + mOrder, item, itemSize);
		mOrder += itemSize;
		countRecords++;
		return this;
	}

	mNextBlock = new cDataBlock<TItem>();
	block++;
	return mNextBlock->Add(item, block);
}

template<class TItem>
inline bool cDataBlock<TItem>::Find(const TItem & item)
{
	int blockSize = 0;
	for (int i = 0; i < countRecords; i++)
	{
		//porovnani mData s term itemem
		if (cTerm::Compare(mData + blockSize, item))
			return true;

		blockSize += cTerm::GetSize(mData + blockSize);
	}

	return false;
}

template<class TItem>
inline char * cDataBlock<TItem>::GetData()
{
	return mData;
}

template<class TItem>
inline unsigned int cDataBlock<TItem>::Getmorder()
{
	return mOrder;
}

template <class TItem>
inline int cDataBlock<TItem>::GetFreeSpace()
{
	return mCapacity - mOrder;
}

