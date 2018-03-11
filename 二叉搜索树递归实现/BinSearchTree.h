#pragma once

#include<iostream>
using namespace std;

template<typename T>
struct BinSearchNode
{
	BinSearchNode<T>* _pLeft;
	BinSearchNode<T>* _pRight;
	T _data;
	BinSearchNode(const T& data = T())
		:_pLeft(NULL)
		, _pRight(NULL)
		, _data(data)
	{}
};

template<typename T>
class BinSearchTree
{
	typedef BinSearchNode<T> Node;
	typedef Node* pNode;
public:
	BinSearchTree()
		:_pRoot(NULL)
	{}

	bool Insert(const T& data)
	{
		return _Insert(_pRoot, data);
	}

	pNode Find(const T& data)
	{
		return _Find(_pRoot, data);
	}

	bool Delete(const T& data)
	{
		return _Delete(_pRoot, data);
	}

	void Inder()
	{
		if (NULL == _pRoot)
			return;
		cout << "�������:";
		_Inder(_pRoot);
		cout << endl;
	}
private:

	void _Inder(pNode pRoot)
	{
		if (NULL == pRoot)
			return;
		_Inder(pRoot->_pLeft);
		cout << pRoot->_data << " ";
		_Inder(pRoot->_pRight);
	}

	bool _Delete(pNode& pRoot, const T& data)
	{
		if (NULL == pRoot)
			return false;
		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)//ֻ��һ���ڵ�
		{
			if (data == pRoot->_data)
			{
				delete pRoot;
				pRoot = NULL;
				return true;
			}
			else
				return false;
		}
		if (data < pRoot->_data)
			return _Delete(pRoot->_pLeft, data);
		else if (data > pRoot->_data)
			return _Delete(pRoot->_pRight,data);
		else
		{
			pNode pDel = pRoot;
			if (NULL == pRoot->_pLeft)//��ǰ������������Ϊ��,�ø����ָ�������������
			{
				pRoot = pRoot->_pRight;
				delete pDel;
				pDel = NULL;
				return true;
			}
			else if (NULL == pRoot->_pRight)
			{
				pRoot = pRoot->_pLeft;
				delete pDel;
				pDel = NULL;
				return true;
			}
			else//������������Ϊ��
			{
				//�ҵ���������������߽ڵ�-������,�滻�����
				pNode pDel = pRoot;
				pDel = pRoot->_pRight;
				while (pDel->_pLeft)
					pDel = pDel->_pLeft;
				
				pRoot->_data = pDel->_data;
				//ɾ��������
				return _Delete(pRoot->_pRight,pDel->_data);
			}

		}
	}

	pNode _Find(pNode pRoot, const T& data)
	{
		if (NULL == pRoot)
			return NULL;

		if (data == pRoot->_data)
			return pRoot;
		else if (data < pRoot->_data)
			return _Find(pRoot->_pLeft, data);
		else
			return _Find(pRoot->_pRight,data);
	}

	bool _Insert(pNode& pRoot, const T& data)
	{
		if (NULL == pRoot)
		{
			pRoot = new Node(data);
			return true;
		}
		if (data == pRoot->_data)
			return false;
		else if (data < pRoot->_data)
			return _Insert(pRoot->_pLeft, data);
		else
			return _Insert(pRoot->_pRight,data);
	}

private:
	pNode _pRoot;
};


void BinarySearchTreeTest()
{
	BinSearchTree<int> bs;
	//int arr[] = { 5, 7, 8, 9, 6, 3, 4, 1, 0, 2 };
	int arr[] = {5,7,6,9};
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		bs.Insert(arr[i]);
	}
	bs.Inder();
	//bs.Delete(5);
	bs.Delete(9);
	bs.Inder();
}
