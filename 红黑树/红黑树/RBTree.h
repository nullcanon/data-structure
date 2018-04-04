#pragma once
#include<utility>
#include<algorithm>
#include<iostream>

using namespace std;

enum Color
{
	RED = 0,
	BLACK
};

template<typename K, typename V>
struct RBTreeNode
{
	RBTreeNode* _pLeft;
	RBTreeNode* _pRight;
	RBTreeNode* _pParent;
	Color _color;
	pair<K, V> _v;
	RBTreeNode(const K& key, const V& value)
		:_pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _color(RED)
		, _v(pair<K, V>(key, value))
	{}
};

template<typename K, typename V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
	typedef Node* pNode;
public:
	RBTree()
		:_pRoot(NULL)
	{}

	bool insert(const K& key, const V& value)
	{
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = BLACK;
			return true;
		}

		pNode pCur = _pRoot;
		pNode pParent = NULL;
		while (pCur)
		{
			pParent = pCur;
			if (pCur->_v.first > key)
				pCur = pCur->_pLeft;
			else if (pCur->_v.first < key)
				pCur = pCur->_pRight;
			else
				return false;
		}
		pCur = new Node(key, value);
		if (pParent->_v.first > key)
			pParent->_pLeft = pCur;
		else
			pParent->_pRight = pCur;

		pCur->_pParent = pParent;

		if (BLACK == pParent->_color)
			return true;
		while (pCur != _pRoot && RED == pParent->_color)
		{
			pNode pGrdftr = pParent->_pParent;

			//根据p位于g的方向
			if (pParent == pGrdftr->_pLeft)
			{
				//叔叔是否存在,节点的颜色
				pNode pUncle = pGrdftr->_pRight;
				if (NULL != pUncle && RED == pUncle->_color)
				{
					pParent->_color = BLACK;
					pUncle->_color = BLACK;
					pGrdftr->_color = RED;
					pCur = pGrdftr;
					pParent = pCur->_pParent;
				}
				else
				{
					//根据cur在p的哪边
					if (pCur == pParent->_pLeft)
					{
						_RotateRight(pParent->_pParent);
						pParent->_color = BLACK;
						pGrdftr->_color = RED;
					}
					else
					{
						_RotateLeft(pParent);
						swap(pParent, pCur);
					}
				}
			}
			else
			{
				//叔叔是否存在,节点的颜色
				pNode pUncle = pGrdftr->_pLeft;
				if (NULL != pUncle && RED == pUncle->_color)
				{
					pParent->_color = BLACK;
					pUncle->_color = BLACK;
					pGrdftr->_color = RED;
					pCur = pGrdftr;
					pParent = pCur->_pParent;
				}
				else
				{
					//根据cur在p的哪边
					if (pCur == pParent->_pRight)
					{
						_RotateLeft(pParent->_pParent);
						pParent->_color = BLACK;
						pGrdftr->_color = RED;
					}
					else
					{
						_RotateRight(pParent);
						swap(pParent, pCur);
					}
				}
			}
		}
		_pRoot->_color = BLACK;
		return true;
	}

	bool IsRBTree()
	{
		if (NULL == _pRoot)
			return true;

		if (BLACK != _pRoot->_color)
			return false;
		int BlackNum = 0;
		int CurBlackNum = 0;
		pNode pCur = _pRoot;

		//先统计出随便一路径的黑色节点的个数
		while (pCur)
		{
			if (BLACK == pCur->_color)
				BlackNum++;
			pCur = pCur->_pLeft;
		}
		return _IsRBTree(_pRoot,BlackNum,CurBlackNum);
	}

	bool Find(const K& key)
	{
		pNode pCur = _pRoot;
		while (pCur)
		{
			if (key == pCur->_v.first)
				return true;
			else if (key < pCur->_v.first)
				pCur = pCur->_pRight;
			else
				pCur = pCur->_pLeft;
		}
		return false;
	}

	bool Remove(const K& key)
	{
		pNode pCur = _pRoot;
		pNode pParent = NULL;
		pNode pDel = NULL;
		
		//寻找要删除的节点
		while (pCur)
		{
			if (key == pCur->_v.first)
				break;
			else if (key < pCur->_v.first)
				pCur = pCur->_pRight;
			else
				pCur = pCur->_pLeft;
		}

		//没找到要删节点
		if (NULL == pCur)
			return false;
		//如果要删除节点有两个孩子节点,转移要删除节点
		if (pCur->_pLeft != NULL && pCur->_pRight != NULL)
		{
			pNode pRightMin = pCur->_pRight;//找到右子树的最小节点
			//即右子树的最左边节点
			while (pRightMin->_pLeft)
				pRightMin = pRightMin->_pLeft;
			pCur->_v = pRightMin->_v;
			pCur = pRightMin;//让pCur指向要删节点
		}
		pParent = pCur->_pParent;
		pDel = pCur;

		//要删除节点的左边为空
		//此逻辑里面提前先处理好父节点的指向
		//在后面的操作中直接删除即可
		if (NULL == pDel->_pLeft)
		{
			//如果要删的是根结点
			if (_pRoot == pDel)
			{
				_pRoot = pDel->_pRight;
				//如果右孩子存在,那它一定为红色
				if (NULL != pDel->_pRight)
				{
					_pRoot->_pParent = NULL;
					_pRoot->_color = BLACK;
				}
				delete pDel;
				return true;
			}

			//让被删节点的父节点指向被删节点的孩子节点
			if (pParent->_pLeft == pDel)
				pParent->_pLeft = pDel->_pRight;
			else
				pParent->_pRight = pDel->_pRight;
			if (pDel->_pRight)
				pDel->_pRight->_pParent = pParent;
			pCur = pDel->_pRight;
		}
		else
		{
			//如果要删的是根结点
			if (_pRoot == pDel)
				_pRoot = pDel->_pLeft;
			//如果左孩子存在,那它一定为红色
			if (NULL != pDel->_pLeft)
			{
				_pRoot->_pParent = NULL;
				_pRoot->_color = BLACK;
			}
			delete pDel;
			return true;

			//让被删节点的父节点指向被删节点的孩子节点
			if (pParent->_pLeft == pDel)
				pParent->_pLeft = pDel->_pRight;
			else
				pParent->_pRight = pDel->_pRight;
			if (pDel->_pRight)
				pDel->_pRight->_pParent = pParent;
			pCur = pDel->_pLeft;//让pCur标记孩子节点
		}

		//如果要删除节点是红色,直接删除
		if (pDel->_color == RED)
		{
			delete pDel;
			return true;
		}

		//如果待删节点为黑的,且其孩子是红的,将孩子改为黑色即可
		if (BLACK == pDel->_color && pCur && RED == pCur->_color)
		{
			pCur->_color = BLACK;
			delete pDel;
			return true;
		}

		//要删除节点是黑色的
		while (pParent)
		{
			//要删除节点是父亲的左孩子
			if (pParent->_pLeft == pDel)
			{
				pNode pSubR = pParent->_pRight;//parent的右孩子,一定存在

				//右孩子是红色的,对应情况①,注意这里的pSubR在图片中表示为S
				if (pSubR->_color == RED)
				{
					_RotateLeft(pParent);
					pSubR->_color = BLACK;
					pParent->_color = RED;
				}
				else//pSubR为黑色
				{
					pNode pSubRL = pSubR->_pLeft;
					pNode pSubRR = pSubR->_pRight;

					//parent为黑色
					//(SubR双节点为空)或者(SubR有双节点并且双节点为黑)
					//对应情况②
					if (BLACK == pParent->_color && (NULL == pSubR->_pLeft && NULL == pSubR->_pRight)
						|| (pSubRL && pSubRR && BLACK == pSubRL->_color && BLACK == pSubRR->_color))
					{
						pSubR->_color = RED;
						pCur = pParent;
						pParent = pCur->_pParent;
					}
					else
					{
						if (RED == pParent->_color)//父节点为红色
						{
							//如果SubR的孩子们为黑 或者没有孩子们
							//对应情况③
							if ((NULL == pSubRL && NULL == pSubRR) ||
								(pSubRL && pSubRR && BLACK == pSubRL->_color && BLACK == pSubRR->_color))
							{
								pParent->_color = BLACK;
								pSubR->_color = RED;
								break;
							}
						}

						//对应情况⑤
						if (RED == pSubRL->_color)
						{
							_RotateRight(pSubR);
							pSubR = pSubRL;
						}

						//对应情况④
						_RotateLeft(pParent);
						if (RED == pParent->_color)
							pSubR->_color = RED;
						else
							pSubR->_color = BLACK;

						pParent->_color = BLACK;
						pSubR->_pRight->_color = BLACK;
						break;
					}
				}
			}
			else//要删除节点是父节点右孩子的情况,与上面相反
			{
				pNode pSubL = pParent->_pLeft;

				if (pSubL->_color == RED)
				{
					_RotateRight(pParent);
					pSubL->_color = BLACK;
					pParent->_color = RED;
				}
				else
				{
					pNode pSubLL = pSubL->_pLeft;
					pNode pSubLR = pSubL->_pRight;

					if (BLACK == pParent->_color && (NULL == pSubL->_pLeft && NULL == pSubL->_pRight)
						|| (pSubLL && pSubLR && BLACK == pSubLL->_color && BLACK == pSubLR->_color))
					{
						pSubL->_color = RED;
						pCur = pParent;
						pParent = pCur->_pParent;
					}
					else
					{
						if (RED == pParent->_color)//父节点为红色
						{
							if ((NULL == pSubLL && NULL == pSubLR) ||
								(pSubLL && pSubLR && BLACK == pSubLL->_color && BLACK == pSubLR->_color))
							{
								pParent->_color = BLACK;
								pSubL->_color = RED;
								break;
							}
						}

						//对应情况⑤
						if (RED == pSubLL->_color)
						{
							_RotateLeft(pSubL);
							pSubL = pSubLR;
						}

						//对应情况④
						_RotateRight(pParent);
						if (RED == pParent->_color)
							pSubL->_color = RED;
						else
							pSubL->_color = BLACK;

						pParent->_color = BLACK;
						pSubL->_pLeft->_color = BLACK;
						break;
					}
				}
			}
		}
		_pRoot->_color = BLACK;
		delete pDel;
		return true;
	}

private:
	bool _IsRBTree(pNode pRoot,int BlackNum,int CurBlackNum)
	{
		if (NULL == pRoot)
			return (BlackNum == CurBlackNum);

		if (RED == pRoot->_color && RED == pRoot->_pParent->_color)
			return false;

		if (BLACK == pRoot->_color)
			CurBlackNum++;
		
		return (_IsRBTree(pRoot->_pLeft, BlackNum, CurBlackNum) && _IsRBTree(pRoot->_pRight, BlackNum, CurBlackNum));
	}

	void _RotateRight(pNode& pParent)
	{
		pNode pSubL = pParent->_pLeft;
		pNode ppParent = pParent->_pParent;

		pParent->_pLeft = pSubL->_pRight;
		if (pSubL->_pRight != NULL)
			pSubL->_pRight->_pParent = pParent;
		pSubL->_pRight = pParent;
		pParent->_pParent = pSubL;
		
		if (NULL == ppParent)
		{
			_pRoot = pSubL;
			return;
		}
		else if (pParent == ppParent->_pLeft)
			ppParent->_pLeft = pSubL;
		else
			ppParent->_pRight = pSubL;

		pSubL->_pParent = ppParent;
	}

	void _RotateLeft(pNode& pParent)
	{
		pNode pSubR = pParent->_pRight;
		pNode ppParent = pParent->_pParent;

		pParent->_pRight = pSubR->_pLeft;
		if (pSubR->_pLeft != NULL)
			pSubR->_pLeft->_pParent = pParent;
		pSubR->_pLeft = pParent;
		pParent->_pParent = pSubR;
		pSubR->_pParent = ppParent;
		if (NULL== ppParent)
		{
			_pRoot = pSubR;
			return;
		}
		else if (pParent == ppParent->_pLeft)
			ppParent->_pLeft = pSubR;
		else
			ppParent->_pRight = pSubR;
	}
private:
	pNode _pRoot;
};

void TestInsert()
{
	int arr[] = { 10, 20, 30, 70, 90, 40, 5,6 };
	int sz = sizeof(arr) / sizeof(arr[0]);
	RBTree<int, int> rb;
	for (int i = 0; i < sz; ++i)
	{
		rb.insert(arr[i], i);
	}

	rb.Remove(20);

	if (rb.IsRBTree())
		cout << "是红黑树" << endl;
	else
		cout << "非红黑树" << endl;
}