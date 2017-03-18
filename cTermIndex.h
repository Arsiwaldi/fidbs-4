#pragma once
#include "HashTable.h"

template <class TItem> class cTermIndex :public CHashTable<TItem>
{
public:
	cTermIndex(int avgSize, int itemCount);
};

template <class TItem>
cTermIndex<TItem>::cTermIndex(int avgSize, int itemCount): CHashTable<TItem>(avgSize,itemCount)
{
}
