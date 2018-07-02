#include"comm.h"

//此函数返回一个比当前size大的素数
static unsigned _GetPrime(const unsigned long size)
{
	const int PrimeSize = 28;
	static const unsigned long PrimeTable[PrimeSize] = {
		53ul, 97ul, 193ul, 389ul, 769ul, 1543ul, 3079ul,
		6151ul, 12289ul, 24593ul, 49157ul, 98317ul, 196613ul, 
		393241ul,786433ul,1572869ul, 3145739ul, 6291469ul, 
		12582917ul,25165843ul,50331653ul, 100663319ul, 201326611ul,
		402653189ul,805306457ul,1610612741ul, 3221225473ul, 4294967291ul
	};
	for (int i = 0; i < PrimeSize; ++i)
	{
		if (size < PrimeSize)
			return PrimeTable[i];
	}
	return PrimeTable[PrimeSize-1];
}