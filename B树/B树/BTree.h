#pragma once

#include<utility> //pair
#include<iostream>
using namespace std;

template<typename K,int M>
struct BTreeNode
{
	K _keys[M];//表示关键字数组，M是孩子的个数，多开辟一个便于处理
	BTreeNode<K, M>* _subs[M + 1];//孩子指针数组
	BTreeNode<K, M>* _parent;//指向父亲的指针
	int _size;//关键字的个数

	BTreeNode()
		:_parent(NULL)
		, _size(0)
	{
		for (int i = 0; i < M; ++i)
		{
			_keys[i] = K();
			_subs[i] = NULL;
		}
		_subs[M] = NULL;
	}
};

template<typename K,int M>
class BTree
{
	typedef BTreeNode<K, M> Node;
	typedef Node* pNode;
public:
	BTree()
		: _pRoot(NULL)
	{}

	~BTree()
	{
		_Destroy();
	}

	// 返回一组数据，
	//如果找到了，frist表示节点，second表示在结点中的位置
	//如果没找到，first表示该数据应该在树中节点，second为-1
	pair<pNode, int> Find(const K& key)
	{
		pNode pCur = _pRoot;
		pNode pParent = NULL;
		while (pCur)
		{
			int i = 0;
			while (i < pCur->_size)
			{
				if (pCur->_keys[i] < key)
					++i;
				else if (pCur->_keys[i] > key)
					break;
				else
					return pair<pNode, int>(pCur, i);//已经存在
			}
			pParent = pCur;
			pCur = pCur->_subs[i];
		}
		return pair<pNode, int>(pParent, -1);
	}

	bool Insert(const K& key)
	{
		//如果是空树
		if (NULL == _pRoot)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size++;
			_pRoot->_parent = NULL;
		}
		//如果不是空树，找key是否存在，重复默认插入失败
		pair<pNode, int> ret = Find(key);
		if (ret.second != -1)//已经存在
			return false;
		pNode pCur = ret.first;
		pNode pSub = NULL;
		K newKey = key;
		while (true)
		{
			_InsertKey(pCur, newKey, pSub);//将关键值插入
			//如果_size已经大于M-1,则需要进行分裂
			//分裂规则：中间元素上移到双亲节点，左右的元素作为两个新的节点
			//			如果双亲节点等于M则需要分裂双亲节点
			if (pCur->_size == M)
			{
				pNode newNode = new Node;
				int mid = pCur->_size >> 1;
				int j = 0, i = mid + 1;
				//把中间元素以后的元素和孩子搬移到新节点
				for (; i < pCur->_size; ++i, ++j)
				{
					newNode->_keys[j] = pCur->_keys[i];
					newNode->_subs[j] = pCur->_subs[i];
					if (newNode->_subs[j])
						newNode->_subs[j]->_parent = newNode;
					pCur->_keys[i] = K();
					pCur->_subs[i] = NULL;
					newNode->_size++;
				}
				//复制最后一个右孩子
				newNode->_subs[j] = pCur->_subs[i];
				if (newNode->_subs[j])
					newNode->_subs[j]->_parent = newNode;
				pCur->_subs[i] = NULL;

				newKey = pCur->_keys[mid];
				pCur->_keys[mid] = K();
				pSub = newNode;
				pCur->_size = mid;

				//如果分裂的是根结点
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

	void Inorder()
	{
		_Inorder(_pRoot);
		cout << endl;
	}

	bool Remove(const K& k)
	{
		if (NULL == _pRoot)
			return false;
		pair<pNode, int> ret = Find(k);
		if (ret.second == -1)
			return false;
		pNode pCur = ret.first;
		int pos = ret.second;
		//判断要删除的是不是叶子节点，如果不是则将其交换到叶子中
		//如果待删节点的右子树不为空，则待删节点为非叶子节点
		if (pCur->_subs[pos + 1] != NULL)
		{
			//找到右子树的最小值和其替换
			pNode pMinKey = pCur->_subs[pos + 1];
			while (pMinKey->_subs[0])
				pMinKey = pMinKey->_subs[0];
			pCur->_keys[pos] = pMinKey->_keys[0];
			pCur = pMinKey;
			_MoveLeft(pCur, 0);//删除交换后的pCur
		}
		else
			_MoveLeft(pCur, pos);

		//判断是否满足B树，不满足则调整
		//求出关键字下限数，M/2-1上取整
		int mid = ((M + 1) >> 1) - 1;
		while (true)
		{
			//若关键码的个数小于上限值，则需要进行调整
			if (pCur->_size < mid)
			{
				if (pCur == _pRoot)
					break;
				pNode pParent = pCur->_parent;
				pos = 0;

				//找到当前节点在父节点中的位置
				while (pParent->_subs[pos] != pCur && pos < pParent->_size)
					++pos;
				if (pos == 0)//当前节点在他父亲的左边，因此与右结点进行调整
					_LeftAdjust(pParent, pCur, mid, pos);
				else
					_RightAdjust(pParent, pCur, mid, pos-1);
				pCur = pParent;
			}
			else
				break;
		}
		//如果根结点的数等于0，则删除根结点，把它的孩子当做根
		if (_pRoot->_size == 0)
		{
			pNode pDel = _pRoot;
			_pRoot = _pRoot->_subs[0];
			if (_pRoot)
				_pRoot->_parent = NULL;
			delete pDel;
		}

		return true;
	}
private:
	void _Destory(pNode pCur)
	{
		if (pCur == NULL)
			return;
		for (int i = 0; i < pCur->_size; ++i)
		{
			_Destory(pCur->_subs[i]);
			delete pCur->_subs[i];
		}
		_Destory(pCur->_subs[i]);
		delete pCur->_subs[i];
	}

	void _LeftAdjust(pNode pParent, pNode pCur, int mid, int pos)
	{
		//如果当前节点的右结点的关键码大于关键码上限，可以收养
		pNode pRight = pParent->_subs[pos + 1];
		if (pRight->_size > mid)
		{
			//将父节点中刚好大于被删除关键码的关键码下移到被删除位置
			pCur->_size++;
			pCur->_keys[pCur->_size - 1] = pParent->_keys[pos];
		
			//将右兄弟中最小的关键码上移到父节点中k的位置
			pParent->_keys[pos] = pRight->_keys[0];

			//将右兄弟中的最左子树的指针平移到被删除关键码所在结点中的最后位置
			pCur->_subs[pCur->_size] = pRight->_subs[0];
			if (pRight->_subs[0])
				pRight->_subs[0]->_parent = pCur;
			pRight->_subs[0] = pRight->_subs[1];
			//在右兄弟的结点中再将被移走的关键码和指针位置用剩余的关键码和指针填补，调整。
			_MoveLeft(pRight, 0);
		}
		else//通过合并解决
			_Merge(pParent, pCur, pRight, pos);

	}

	void _RightAdjust(pNode pParent, pNode pCur, int mid, int pos)
	{
		pNode pLeft = pParent->_subs[pos];
		if (pLeft->_size > mid)
		{
			_MoveRight(pCur, 0);
			pCur->_keys[0] = pParent->_keys[pos];
			pParent->_keys[pos] = pLeft->_keys[pLeft->_size - 1];
			pCur->_subs[0] = pLeft->_subs[pLeft->_size];
			if (pLeft->_subs[pLeft->_size])
				pLeft->_subs[pLeft->_size]->_parent = pCur;

			pLeft->_keys[pLeft->_size - 1] = K();
			pLeft->_subs[pLeft->_size] = NULL;
			pLeft->_size--;
		}
		else
			_Merge(pParent, pCur, pLeft, pos);
	}

	void _MoveLeft(pNode pCur, int pos)
	{
		if (pCur == NULL)
			return;
		int i = 0;
		for (; i < pCur->_size; ++i)
		{
			pCur->_keys[i] = pCur->_keys[i + 1];
			pCur->_subs[i + 1] = pCur->_subs[i + 2];
		}
		pCur->_size--;
	}

	void _MoveRight(pNode pCur, int pos)
	{
		int i = pCur->_size - 1;
		for (; i >= pos; --i)
		{
			pCur->_keys[i + 1] = pCur->_keys[i];
			pCur->_subs[i + 2] = pCur->_subs[i + 1];
		}
		pCur->_subs[1] = pCur->_subs[0];
		pCur->_size++;
	}

	void _Merge(pNode pParent, pNode pCur, pNode pBrother, int pos)
	{
		int i = pCur->_size;
		//把父节点相应的值的关键码移到左孩子中
		pCur->_keys[i] = pParent->_keys[pos];
		//把右兄弟最左边的孩子移过来
		pCur->_subs[i + 1] = pBrother->_subs[0];
		if (pBrother->_subs[0])
			pBrother->_subs[0]->_parent = pCur;
		i++;
		pCur->_size++;
		//将兄弟节点指针拷贝过来
		for (int j = 0; j < pBrother->_size; ++i, ++j)
		{
			pCur->_keys[i] = pBrother->_keys[j];
			pCur->_subs[i + 1] = pBrother->_subs[j + 1];
			if (pBrother->_subs[j + 1])
				pBrother->_subs[j + 1]->_parent = pCur;
			pCur->_size++;
		}
		if (pParent->_subs[pos] == pBrother)
			pParent->_subs[pos] = NULL;
		else
			pParent->_subs[pos + 1] = NULL;
		_MoveLeft(pParent, pos);
		delete pBrother;
	}

	void _Inorder(pNode pRoot)
	{
		if (pRoot == NULL)
			return;
		//参考二叉树的中序
		//只不过B树需要循环输出节点里面的值
		int i = 0;
		for (; i < pRoot->_size; ++i)
		{
			_Inorder(pRoot->_subs[i]);
			cout << pRoot->_keys[i] << " ";
		}
		
		_Inorder(pRoot->_subs[i]);
	}

	void _InsertKey(pNode pCur, const K& key, pNode pSub)
	{
		int i = pCur->_size - 1;
		while (i >= 0)//将key插入到pCur节点中
		{
			if (key < pCur->_keys[i])//从后向前遍历并搬移pCur节点的值和孩子
			{
				pCur->_keys[i + 1] = pCur->_keys[i];
				pCur->_subs[i + 2] = pCur->_subs[i + 1];
				--i;
			}
			else
				break;
		}
		//把key插入到相应位置
		pCur->_keys[i + 1] = key;
		pCur->_subs[i + 2] = pSub;
		if (pSub != NULL)
			pSub->_parent = pCur;
		pCur->_size++;
	}
private:
	pNode _pRoot;
};

void TestInsert()
{
	BTree<int, 3> bt;
	int arr[] = { 53, 75, 139, 49, 145, 36, 101 };
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		bt.Insert(arr[i]);
	}
	bt.Inorder();
}

void TestRemove()
{
	BTree<int, 3> bt;
	int arr[] = { 53, 75, 139, 49, 145, 36, 101 };
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		bt.Insert(arr[i]);
	}
	bt.Remove(49);
	bt.Inorder();
}