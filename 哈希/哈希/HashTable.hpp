//闭散列法
#pragma once
#include<cstring>
#include<vector>
#include<iostream>
#include"comm.h"
using namespace std;
enum Status
{
	EXIST,//存在元素
	EMPTY,//位置为空
	DELETE//元素被删除
};
const size_t capacity = 10;

template<typename K,typename V>
struct HashTableElement
{
	K _key;
	V _value;
	Status _status;
	HashTableElement(const K& key = K(), const V& value = V())
		:_key(key)
		, _value(value)
		, _status(EMPTY)
	{}
};


//为了处理string或者别的类型,这里增加了配置器
template<typename K,typename V,class GetV = _GetV<K> >
class HashTable
{
	typedef HashTableElement<K,V> Element;
public:
	HashTable()
		: _num(0)
	{
		_Table.resize(_GetPrime(0));
	}

	bool Insert(const K& key,const V& value)
	{
		_CheckCapacity();
		int index = _HashFuc(key);

		if (index == -1)
			return false;

		while (EMPTY != _Table[index]._status)
		{
			if (DELETE == _Table[index]._status)
				return false;

			//线性探测
			++index;

			//二次探测
			//index = index*index - 1;

			if (index >= (int)_Table.size())
				index %= _Table.size();
		}
		_Table[index]._value = value;
		_Table[index]._key = key;
		_Table[index]._status = EXIST;
		++_num;
		return true;
	}
	
	int Find(const K& key)
	{
		int index = _HashFuc(key);
		if (index == -1)
			return -1;
		while (EMPTY != _Table[index]._status)
		{
			if (key == _Table[index].key)
				return index;

			++index;
			if (index >= (int)_Table.size())
				index %= _Table.size();
		}
		return -1;
	}

	bool Remove(const K& key)
	{
		int index = Find(value);
		if (-1 == index)
			return false;
		_Table[index]._status = DELETE;
		--_num;
		return true;
	}

	void Swap(HashTable<K,V>& hash)
	{
		if (this != &hash)
		{
			_Table.swap(hash._Table);
			swap(_num, hash._num);
		}
	}
private:
	int _HashFuc(const K& value)
	{
		if (!_Table.empty())
			return (GetV()(value))%_Table.size();
		return -1;
	}

	////静态函数,与对象无关,在对象没有生成的时候就能调用
	////此函数返回一个比当前size大的素数
	//static unsigned _GetPrime(const unsigned long size)
	//{
	//	const int PrimeSize = 28;
	//	static const unsigned long PrimeTable[PrimeSize] = {
	//		53ul, 97ul, 193ul, 389ul, 769ul, 1543ul, 3079ul,
	//		6151ul, 12289ul, 24593ul, 49157ul, 98317ul, 196613ul, 
	//		393241ul,786433ul,1572869ul, 3145739ul, 6291469ul, 
	//		12582917ul,25165843ul,50331653ul, 100663319ul, 201326611ul,
	//		402653189ul,805306457ul,1610612741ul, 3221225473ul, 4294967291ul
	//	};
	//	for (int i = 0; i < PrimeSize; ++i)
	//	{
	//		if (size < PrimeSize)
	//			return PrimeTable[i];
	//	}
	//	return PrimeTable[PrimeSize-1];
	//}

	void _CheckCapacity()
	{
		//载荷因子设置为0.7
		if (_Table.size() == 0 || _num*10/_Table.size() >= 7)
		{
			int newSize = _GetPrime(_Table.size());
			HashTable<K,V> hash;
			hash._Table.resize(newSize);
			for (size_t i = 0; i < _Table.size(); ++i)
			{
				if (_Table[i]._status == EXIST)
					hash.Insert(_Table[i]._value,_Table[i]._key);
			}
			Swap(hash);
		}
	}
private:
	vector<Element> _Table;
	size_t _num;//实际个数
};

void TestHashTable()
{
	HashTable<int,int> hash;
	
	hash.Insert(13,13);
	hash.Insert(44,44);
	hash.Insert(23,44);

	/*hash.Delete(13);
	hash.Delete(23);*/
}