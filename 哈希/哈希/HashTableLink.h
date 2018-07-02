//开散列法
#pragma once
#include<cstring>
#include<vector>
#include<iostream>
#include"comm.h"

template<typename K,typename V>
struct HashTableNode
{
	K _key;
	V _value;
	HashTableNode<K, V>* _pNext;
	HashTableNode(const K& key=K(),const V& value=V())
		:_key(key)
		, _value(value)
		, _pNext(NULL)
	{}
};

template<typename K, typename V, class GetV = _GetV<K> >
class HashTableLink
{
	typedef HashTableNode<K, V> Node;
	typedef Node* pNode;
public:
	HashTableLink()
		:_size(0)
	{
		_tables.resize(_GetPrime(0));
	}

	pNode Find(const K& key)
	{
		int index = _HashFuc(key);
		if (index > 0)
		{
			pNode pCur = _tables[index];
			while (pCur)
			{
				if (pCur->_key == key)
					return pCur;

				pCur = pCur->_pNext;
			}
		}
		return NULL;
	}

	bool Insert(const K& key, const V& value)
	{
		_CheckCapacity();
		pNode ret = Find(key);
		if (ret == NULL)
		{
			int index = _HashFuc(key);
			pNode pCur = new Node(key, value);
			pCur->_pNext = _tables[index];
			_tables[index] = pCur;
			_size++;
			return true;
		}
		return false;
	}

	bool Remove(const K& key)
	{
		pNode pCur = Find(key);
		if (pCur != NULL)
		{
			pNode pPre = NULL;
			while (pCur)
			{
				if (pCur->_key == key)
				{
					if (pPre == NULL)
					{
						//链表的头
						_tables[_HashFuc(key)] = pCur->_pNext;
					}
					else
					{
						pPre->_pNext = pCur->_pNext;
					}
					delete pCur;
					_size--;
					return true;
				}
				pPre = pCur;
				pCur = pCur->_pNext;
			}
		}
		return false;
	}
	
	void Swap(HashTableLink<K, V, GetV>& hash)
	{
		_tables.swap(hash._tables);
		swap(_size, hash._size);
	}
private:
	void _CheckCapacity()
	{
		int size = _tables.size();
		if (size == 0 || _size == size)
		{
			HashTableLink<K, V, GetV> hash;
			hash._tables.resize(_GetPrime(size));
			for (int i = 0; i < size; ++i)
			{
				pNode pCur = _tables[i];
				while (pCur)
				{
					hash.Insert(pCur->_key, pCur->_value);
					pCur = pCur->_pNext;
				}
			}
			Swap(hash);
		}
	}

	int _HashFuc(const K& value)
	{
		if (!_tables.empty())
			return (GetV()(value)) % _tables.size();
		return -1;
	}
private:
	vector<pNode> _tables;
	size_t _size;
};

void TestHashTableLink()
{
	HashTableLink<int, int> hash;

	hash.Insert(13, 13);
	hash.Insert(44, 44);
	hash.Insert(23, 44);
	hash.Insert(13, 13);

	hash.Remove(13);
	hash.Remove(23);
}