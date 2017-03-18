#pragma once
#include "cDataBlock.h"

template <class TItem> class CHashTable
{
private:
	//Size of table
	unsigned int mSize;
	//Maximum length in block to hash value
	unsigned mMaxStringLength = 0;
	//block size used in cDataBlock
	unsigned int blockSize = 8192;
	//latesdId
	unsigned int latestId;
	//noninserted
	unsigned int nCount;
	//average item size
	unsigned int avgItemSize = 0;
	//item count
	unsigned int itemCount = 0;
public:
	//Count of inserted items
	unsigned int mCount;

	cDataBlock<TItem>** mHashTable;
	cDataBlock<TItem>** mLastBlock;
	int * statsArray;
	int GetLatestid() const;
	void SetLatestId(char* item);

	CHashTable(int avgItemSize, int itemCount)
	{
		mCount = 0;
		nCount = 0;
		mSize = ((float)itemCount / ((blockSize / (float)avgItemSize)*0.9));
		mHashTable = new cDataBlock<TItem>*[mSize]();
		mLastBlock = new cDataBlock<TItem>*[mSize]();
		statsArray = new int[mSize]();
		latestId = 0;
		this->avgItemSize = avgItemSize;
		this->itemCount = itemCount;

		/*for (unsigned int i = 0; i < mSize; i++)
		{
			mHashTable[i] = new cDataBlock<TItem>();
			mLastBlock[i] = new cDataBlock<TItem>();
			statsArray[i] = 0;
		}*/
	}

	template <typename TItem>
	inline void Insert(const TItem &item)
	{
		unsigned int indexHash = Hash(cTerm::Hash(item));

		if (mHashTable[indexHash] == NULL)
		{
			mHashTable[indexHash] = new cDataBlock<TItem>();
			mLastBlock[indexHash] = mHashTable[indexHash];
			statsArray[indexHash] = ++mMaxStringLength;
			mMaxStringLength--;
		}

		if (!Find(item))
		{
			SetLatestId(item);
			mLastBlock[indexHash] = mLastBlock[indexHash]->Add(item, statsArray[indexHash]);
			statsArray[indexHash]++;
			mCount++;
		}
		else
		{
			nCount++;
		}
	}

	template<typename TItem>
	inline bool Find(const TItem &item)
	{

		unsigned int indexHash = Hash(cTerm::Hash(item));

		cDataBlock<TItem> * next = mLastBlock[indexHash];

		do {
			//Find nad celým term itemem
			if (next->Find(item))
			{
				return true;
			}

		} while ((next = next->mNextBlock) != NULL);

		return false;
	}

	inline unsigned int Hash(unsigned int hash) const
	{
		return hash % mSize;
	}

	inline void GetStats()
	{
		int freeSpace = 0;
		for (int i = 0; i < mSize; i++)
		{
			cDataBlock<TItem> * next = mLastBlock[i];
			if(next != nullptr)
			{
				do {
					freeSpace += next->GetFreeSpace();
				} while ((next = next->mNextBlock) != NULL);
			}
		}

		int blockCount = 0;
		for (int i = 0; i < mSize; i++)
		{

			blockCount += statsArray[i];
		}

		int max = 0;
		for (int i = 0; i < mSize; i++)
		{
			if (max < statsArray[i])
				max = statsArray[i];
		}
		cout << "Average item size:" << avgItemSize << " " << "Item count:" << itemCount << endl;
		cout << "Count of inserted items: " << mCount << endl;
		cout << "Count of noninserted items: " << nCount << endl;
		cout << "Table size: " << mSize << endl;
		cout << "Block count: " << blockCount << endl;
		cout << "MaxLengthBlock: " << max << endl;
		cout << "AverageSizeOfBlock: " << static_cast<double>(blockCount) / mSize << endl;
		cout << "AverageUtilization: " << static_cast<double>(1 - (static_cast<double>(freeSpace) / (blockCount * 8192))) * 100.00<< endl;
	}
};

template<class TItem>
inline int CHashTable<TItem>::GetLatestid() const
{
	return latestId;
}

template<class TItem>
inline void CHashTable<TItem>::SetLatestId(char * item)
{
	int ORDER_ID = 0;
	latestId = *reinterpret_cast<unsigned int*>(item + ORDER_ID);
}
