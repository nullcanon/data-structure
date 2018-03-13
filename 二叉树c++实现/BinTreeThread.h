#pragma once

#include<iostream>
using namespace std;

enum pInfo{ LINK, THREAD };

template<class T>
struct BinTreeThreadNode
{
	BinTreeThreadNode(const T data)
	:_pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _data(data)
	, _pLeftThread(LINK)
	, _pRightThread(LINK)
	{}

	BinTreeThreadNode<T>* _pLeft;
	BinTreeThreadNode<T>* _pRight;
	BinTreeThreadNode<T>* _pParent;
	T _data;
	pInfo _pLeftThread;
	pInfo _pRightThread;
};

template <class T>
class BinTreeThread
{
	typedef BinTreeThreadNode<T> Node;
	typedef Node* pNode;
	typedef void(*pFun)(pNode);

public:
	BinTreeThread()
		:_pRoot(NULL)
	{}
	void CreateBinTree(T* arr, size_t size, T invalid)
	{
		size_t index = 0;
		_CreateBinTree(_pRoot, arr, size, index, invalid);
	}
	void PreVisit()//前序遍历
	{
		_PreVisit(_pRoot, _PrintNode);
	}

	void InVisit()//中序遍历
	{
		_InVisit(_pRoot, _PrintNode);
	}

	void PostVisit()//后序遍历
	{
		_PostVisit(_pRoot, _PrintNode);
	}

	void LevelVisit()//层序遍历
	{
		_LevelVisit(_pRoot, _PrintNode);
	}

	void PreThread()
	{
		pNode prev = NULL;
		_PreThread(_pRoot,prev);
	}

	void InThread()
	{
		pNode prev = NULL;
		_InThread(_pRoot,prev);
	}

	void PostThread()
	{
		pNode prev = NULL;
		_PostThread(_pRoot,prev);
	}

	void PreThreadVisit()
	{
		_PreThreadVisit(_pRoot,_PrintNode);
	}

	void InThreadVisit()
	{
		_InThreadVisit(_pRoot,_PrintNode);
	}

	void PostThreadVisit()
	{
		_PostThreadVisit(_pRoot,_PrintNode);
	}
protected:

	void _CreateBinTree(pNode& root, T* arr, size_t size, size_t& index, T invalid)
	{
		if (index < size && invalid != arr[index])
		{
			root = new Node(arr[index]);

			_CreateBinTree(root->_pLeft, arr, size, ++index, invalid);

			if (root->_pLeft)
				root->_pLeft->_pParent = root;
			_CreateBinTree(root->_pRight, arr, size, ++index, invalid);

			if (root->_pRight)
				root->_pRight->_pParent = root;
		}
		else
			root = NULL;
	}

	void _PreVisit(pNode& pd, pFun visit)
	{
		if (pd)
		{
			visit(pd);
			if (pd->_pLeft)
				_PreVisit(pd->_pLeft, visit);
			if (pd->_pRight)
				_PreVisit(pd->_pRight, visit);
		}
	}

	void _InVisit(pNode& pd, pFun visit)
	{
		if (pd)
		{
			if (pd->_pLeft)
				_InVisit(pd->_pLeft, visit);
			visit(pd);
			if (pd->_pRight)
				_InVisit(pd->_pRight, visit);
		}
	}

	void _PostVisit(pNode& pd, pFun visit)
	{
		if (pd)
		{
			if (pd->_pLeft)
				_PostVisit(pd->_pLeft, visit);
			if (pd->_pRight)
				_PostVisit(pd->_pRight, visit);
			visit(pd);
		}
	}

	void _LevelVisit(pNode& pd, pFun visit)
	{
		if (NULL == _pRoot)
			return;
		queue<pNode> q;
		pNode pCur = pd;
		q.push(pd);
		while (!q.empty())
		{
			pCur = q.front();
			visit(pCur);
			if (pCur->_pLeft)
				q.push(pCur->_pLeft);
			if (pCur->_pRight)
				q.push(pCur->_pRight);
			q.pop();
		}
	}

	static void _PrintNode(pNode pd)
	{
		cout << pd->_data << "->";
	}

	void _PreThread(pNode& pd, pNode& prev)
	{
		if (NULL == pd)
			return;

		//线索化当前节点
		if (NULL == pd->_pLeft)
		{
			pd->_pLeft = prev;
			pd->_pLeftThread = THREAD;
		}
		if (prev && NULL == prev->_pRight)
		{
			prev->_pRight = pd;
			prev->_pRightThread = THREAD;
		}
		prev = pd;

		//线索化左子树
		if (LINK == pd->_pLeftThread)
			_PreThread(pd->_pLeft,prev);

		//线索化右子树
		if (LINK == pd->_pRightThread)
			_PreThread(pd->_pRight,prev);		
	}

	void _InThread(pNode& pd, pNode& prev)
	{
		if (NULL == pd)
			return;

		_InThread(pd->_pLeft,prev);

		if (NULL == pd->_pLeft)
		{
			pd->_pLeft = prev;
			pd->_pLeftThread = THREAD;
		}
		if (prev && NULL == prev->_pRight)
		{
			prev->_pRight = pd;
			prev->_pRightThread = THREAD;
		}
		prev = pd;

		if (LINK == pd->_pRightThread)
			_InThread(pd->_pRight,prev);
	}

	void _PostThread(pNode& pd, pNode& prev)
	{
		if (NULL == pd)
			return;

		_PostThread(pd->_pLeft, prev);
		_PostThread(pd->_pRight, prev);

		if (NULL == pd->_pLeft)
		{
			pd->_pLeft = prev;
			pd->_pLeftThread = THREAD;
		}
		if (prev && NULL == prev->_pRight)
		{
			prev->_pRight = pd;
			prev->_pRightThread = THREAD;
		}
		prev = pd;	
	}

	void _PreThreadVisit(pNode pd, pFun visit)
	{
		if (NULL == pd)
			return;
		pNode pCur = pd;

		while (pCur)
		{
			while (LINK == pCur->_pLeftThread)
			{
				visit(pCur);
				pCur = pCur->_pLeft;
			}
			visit(pCur);
			pCur = pCur->_pRight;
		}
	}

	void _InThreadVisit(pNode pd, pFun visit)
	{
		if (NULL == pd)
			return;

		pNode pCur = pd;

		while (pCur)
		{
			while (LINK == pCur->_pLeftThread)
				pCur = pCur->_pLeft;
			visit(pCur);
			while (pCur && THREAD == pCur->_pRightThread)
			{
				pCur = pCur->_pRight;
				visit(pCur);
			}

			pCur = pCur->_pRight;
		}
	}
	
	void _PostThreadVisit(pNode pd, pFun visit)
	{
		if (NULL == pd)
			return;

		pNode pCur = pd;
		pNode prev = NULL;
		while (pCur)
		{
			//找到最左边的节点
			while (prev != pCur->_pLeft && LINK == pCur->_pLeftThread)
				pCur = pCur->_pLeft;

			while (pCur && THREAD == pCur->_pRightThread)
			{
				visit(pCur);
				prev = pCur;
				pCur = pCur->_pRight;
			}

			if (pCur == pd && (prev == pd->_pRight || NULL == pd->_pRight))
			{
				visit(pCur);
				return;
			}

			while (pCur&& prev == pCur->_pRight)
			{
				visit(pCur);
				prev = pCur;
				pCur = pCur->_pParent;
			}

			if (pCur && LINK == pCur->_pRightThread)
				pCur = pCur->_pRight;
		}
	}
	

private:
	pNode _pRoot;
};


void TestBinTreeThread()
{
	BinTreeThread<char> bt;
	char arr[] = "ABD###CE##F";/*
							    A
							   / \
							  B   C
							 /	  /\
							D    E  F
							   */
	bt.CreateBinTree(arr, strlen(arr), '#');//创建树
	//bt.PreVisit();
	//bt.PreThread();
	//cout << endl;
	/*bt.InVisit();
	bt.InThread();*/
	//bt.PostThread();
	//bt.PreThreadVisit();

	
	/*cout << endl;
	bt.InThreadVisit();*/

	bt.PostVisit();
	cout << endl;
	bt.PostThread();
	bt.PostThreadVisit();
}