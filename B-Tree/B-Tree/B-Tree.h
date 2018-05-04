#pragma once

#include<iostream>
using namespace std;

template<typename K,int M>
struct BTreeNode
{
	K _keys[M];
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;
	size_t _size;
	BTreeNode()
		:_size(0)
		, _parent(NULL)
	{
		for (int i = 0; i < M; ++i)
		{
			_keys[i] = K();
			_subs[i] = NULL;
		}
		_subs[M] = NULL;
	}
};

template<typename K, int M>
class BTree
{
	typedef BTreeNode<K, M> Node;
	typedef Node* pNode;
public:
	BTree()
		:_pRoot(NULL)
	{}

	pair<pNode, int> Find(const K& key)
	{
		pNode pCur = _pRoot;
		pNode pParent = pCur;
		while (pCur)
		{
			size_t i = 0;
			for (; i < pCur->_size;)
			{
				if (pCur->_keys[i] == key)
					return pair<pNode,int>(pCur, i);
				else if (pCur->_keys[i] > key)
					break;
				else
					++i;
			}
			pParent = pCur;
			pCur = pCur->_subs[i];
		}
		return pair<pNode,int>(pParent, -1);
	}

	bool Insert(const K& key)
	{
		if (_pRoot == NULL)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size++;
			_pRoot->_parent = NULL;
		}

		pair<pNode, int> ret = Find(key);
		if (-1 != ret.second)
			return false;

		pNode pCur = ret.first;
		pNode pSub = NULL;
		K newKey = key;
		while (true)
		{
			_InsertKey(pCur, newKey, pSub);
			if (pCur->_size == M)//需要分裂
			{
				pNode newNode = new Node;
				int mid = pCur->_size >> 1;
				int i = mid + 1, j = 0;
				for (; i < (int)pCur->_size; ++i, ++j)
				{
					newNode->_keys[j] = pCur->_keys[i];
					newNode->_subs[j] = pCur->_subs[i];
					if (newNode->_subs[j] != NULL)
						newNode->_subs[j]->_parent = newNode;
					pCur->_keys[i] = K();
					pCur->_subs[i] = NULL;
					newNode->_size++;
				}
				//复制最后一个孩子
				newNode->_subs[j] = pCur->_subs[i];
				if (newNode->_subs[j])
					newNode->_subs[j]->_parent = newNode;
				pCur->_subs[i] = NULL;

				newKey = pCur->_keys[mid];
				pCur->_keys[mid] = K();
				pSub = newNode;
				pCur->_size = mid;

				//分裂的节点是根结点
				if (pCur == _pRoot)
				{
					_pRoot = new Node;
					_pRoot->_keys[0] = newKey;
					_pRoot->_subs[0] = pCur;
					_pRoot->_subs[1] = pSub;
					pCur->_parent = _pRoot;
					pSub->_parent = _pRoot;
					_pRoot->_size = 1;
					return true;
				}
				pCur = pCur->_parent;
			}
			else
				break;
		}
		return true;
	}

	void Inder()
	{
		_Inder(_pRoot);
	}
private:
	void _Inder(pNode pRoot)
	{
		if (pRoot == NULL)
			return;
		for (int i = 0; i < (int)pRoot->_size; ++i)
		{
			_Inder(pRoot->_subs[i]);
			cout << pRoot->_keys[i] << " ";
		}
		_Inder(pRoot->_subs[pRoot->_size]);
	}
	void _InsertKey(pNode pCur, const K& key, pNode pSub)
	{
		int i = pCur->_size - 1;
		while (i>=0 && key < pCur->_keys[i])
		{
			pCur->_keys[i + 1] = pCur->_keys[i];
			pCur->_subs[i + 2] = pCur->_subs[i + 1];
			--i;
		}
		pCur->_keys[i+1] = key;
		pCur->_subs[i + 2] = pSub;
		if (pSub != NULL)
			pSub->_parent = pCur;
		pCur->_size++;
	}
private:
	pNode _pRoot;
};


void TestBTree()
{
	BTree<int, 3> bt;
	int arr[] = {53, 75, 139, 49, 145, 36, 101};
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		bt.Insert(arr[i]);
	}
	bt.Inder();
}