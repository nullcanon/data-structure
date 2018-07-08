#pragma once
#include<vector>

using namespace std;

class BitMap
{
public:
	BitMap(size_t range)//位图表示数据的范围
	{
		_bitmap.reserve(range / 32 + 1);
	}

	void Set(size_t num)
	{
		size_t idx = num / 32;
		size_t bit = num % 32;
		_bitmap[idx] |= (1 << bit);
	}

	void ReSet(size_t num)
	{
		size_t idx = num / 32;
		size_t bit = num % 32;
		_bitmap[idx] &= (~(1 << bit));
	}

	bool Test(size_t num)
	{
		size_t idx = num / 32;
		size_t bit = num % 32;
		return (_bitmap[idx] >> num) & 1;
	}
private:
	vector<size_t> _bitmap;
};