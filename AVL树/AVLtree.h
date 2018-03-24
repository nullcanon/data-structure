
#include<iostream>
#include<assert.h>
using namespace std;

template<typename K,typename V>
struct AVLtreeNode
{
	K _key;
	V _value;
	int _bf;//平衡因子
	AVLtreeNode* _pLeft;
	AVLtreeNode* _pRight;
	AVLtreeNode* _pParent;

	AVLtreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _bf(0)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
	{}
};

template<typename K,typename V>
class AVLTree
{
	typedef AVLtreeNode<K,V> Node;
	typedef Node* pNode;
public:
	AVLTree()
		:_pRoot(NULL)
	{}

	bool Find(const K& key)
	{
		Node* pCur = _pRoot;
		while (pCur)
		{
			if (pCur->_key < key)
				pCur = pCur->_right;
			else if (pCur->_key>key)
				pCur = pCur->_left;
			else
				return true;
		}
		return false;
	}

	bool Insert(const K& key, const V& value)
	{
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key,value);
			return true;
		}
		pNode pParent = NULL;
		pNode pCur = _pRoot;
		while(pCur) 
		{
			if (key > pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pRight;
			}
			else if (key < pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pLeft;
			}
			else
				return false;
		}
		pCur = new Node(key, value);
		if (key > pParent->_key)//插入到parent的右边
		{
			pParent->_pRight = pCur;
			pParent->_bf++;
		}			
		else if (key < pParent->_key)//插入到parent的左边
		{
			pParent->_pLeft = pCur;
			pParent->_bf--;
		}
			
		pCur->_pParent = pParent;

		//从插入节点到根结点回溯,更新平衡因子
		//如果某棵子树不平衡,旋转使之平衡
		while (pParent)
		{
			if (0 == pParent->_bf)//树的高度没变,直接返回
				break;
			else if (1 == pParent->_bf || -1 == pParent->_bf)//当前不需要旋转,继续向上查找
			{
				pNode ppParent = pParent->_pParent;
				if (ppParent != NULL)
				{
					if (pParent == ppParent->_pLeft)//表明ppParent的左子树长高了
						ppParent->_bf--;
					else
						ppParent->_bf++;
				}
				pParent = ppParent;//继续向上判断
			}
			else//树需要旋转,分情况讨论
			{
				//parent分两种情况,2或-2
				if (2 == pParent->_bf)
				{
					//左单旋或者先右旋再左旋
					//如果subR为1左单旋,subR为-1右左旋
					if (1 == pParent->_pRight->_bf)
						_RotateL(pParent);
					else
						_RotateRL(pParent);
				}
				else if (-2 == pParent->_bf)
				{
					//右单旋或者先左旋在右旋
					//如果subL为-1右单旋,subL为1左右旋
					if (-1 == pParent->_pLeft->_bf)
						_RotateR(pParent);
					else
						_RotateLR(pParent);
				}
				break;//旋转之后树新的父节点的平衡因子为0,不需要在向上更新
			}
		}
		return true;
	}

	void Inder()
	{
		if (NULL == _pRoot)
			return;
		cout << "中序遍历:";
		_Inder(_pRoot);
		cout << endl;
	}

	bool IsBalance()
	{
		return _IsBalance(_pRoot);
	}

private:
	bool _IsBalance(const pNode pRoot)
	{
		if (NULL == pRoot)
			return true;

		int bf = _Height(pRoot->_pRight) - _Height(pRoot->_pLeft);
		if (abs(bf) > 1 || bf != pRoot->_bf)
			return false;

		return (_IsBalance(pRoot->_pLeft) && _IsBalance(pRoot->_pRight));
	}

	int _Height(const pNode pRoot)
	{
		if (NULL == pRoot)
			return 0;
		int left = _Height(pRoot->_pLeft);
		int right = _Height(pRoot->_pRight);
		return (left > right ? left : right) + 1;
	}

	void _Inder(pNode pRoot)
	{
		if (pRoot != NULL)
		{
			_Inder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_Inder(pRoot->_pRight);
		}
	}

	void _RotateL(pNode &pParent)//左单旋
	{
		assert(pParent != NULL);
		pNode psubR = pParent->_pRight;
		pNode psubRL = psubR->_pLeft;

		pParent->_pRight = psubRL;
		if (psubRL != NULL)
			psubRL->_pParent = pParent;

		psubR->_pLeft = pParent;
		psubR->_pParent = pParent->_pParent;

		if (pParent->_pParent != NULL)
		{
			//如果pParent的是其双亲的左孩子,让其双亲的左指psubR
			if (pParent == pParent->_pParent->_pLeft)
				pParent->_pParent->_pLeft = psubR;
			else
				pParent->_pParent->_pRight = psubR;
		}
		else
			_pRoot = psubR;

		pParent->_pParent = psubR;
		pParent->_bf = psubR->_bf = 0;///更新平衡因子
	}

	void _RotateR(pNode &pParent)//右单旋
	{
		assert(pParent != NULL);
		pNode psubL = pParent->_pLeft;
		pNode psubLR = psubL->_pRight;

		if (psubLR != NULL)
			psubLR->_pParent = pParent;

		pParent->_pLeft = psubLR;
		psubL->_pRight = pParent;
		if (pParent->_pParent != NULL)
		{
			//如果pParent是其双亲的左孩子,让其双亲的左指向psubL
			if (pParent == pParent->_pParent->_pLeft)
				pParent->_pParent->_pLeft = psubL;
			else
				pParent->_pParent->_pRight = psubL;
		}
		else
			_pRoot = psubL;

		pParent->_pParent = psubL;
		pParent->_bf = psubL->_bf = 0;
	}

	void _RotateRL(pNode &pParent)//右左旋
	{
		pNode psubR = pParent->_pRight;
		pNode psubRL = psubR->_pLeft;

		//根据psubRL的bf来判断节点插入的位置
		//从而确定节点旋转后各个节点的bf
		int bf = psubRL->_bf;

		_RotateR(pParent->_pRight);
		_RotateL(pParent);

		if (0 == bf)//要插入的就是subRL本身
		{
			psubR->_bf = pParent->_bf = 0;
		}
		else if(1 == bf)//节点插在了psuRL的右边
		{
			psubR->_bf = 0;
			pParent->_bf = -1;
		}
		else
		{
			psubR->_bf = 1;
			pParent->_bf = 0;
		}
		psubRL->_bf = 0;
	}
	void _RotateLR(pNode &pParent)//左右旋
	{
		pNode psubL = pParent->_pLeft;
		pNode psubLR = psubL->_pRight;
		int bf = psubLR->_bf;

		_RotateL(pParent->_pLeft);
		_RotateR(pParent);

		if (0 == bf)//要插入的就是subLR本身
		{
			psubL->_bf = pParent->_bf = 0;
		}
		else if (1 == bf)//节点插在了psuLR的右边
		{
			psubL->_bf = -1;
			pParent->_bf = 0;
		}
		else//节点插在了subLR的左边
		{
			psubLR->_bf = 0;
			pParent->_bf = 1;
		}
		psubLR->_bf = 0;
	}
private:
	pNode _pRoot;
};

void TestAVLTree1()//测试右左旋
{
	AVLTree<int, int> at;
	int arr[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	int sz = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < sz; ++i)
	{
		at.Insert(arr[i],i);
	}
	at.Inder();
	if (at.IsBalance())
		cout << "BinTree balance" << endl;
	else
		cout << "不平衡" << endl;
}

void TestAVLTree2()//测试左右旋
{
	AVLTree<int, int> at;
	int arr[] = {15,10,20,13,5,1};
	int sz = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < sz; ++i)
	{
		at.Insert(arr[i], i);
	}
	at.Inder();
	if (at.IsBalance())
		cout << "BinTree balance" << endl;
	else
		cout << "不平衡" << endl;
}
