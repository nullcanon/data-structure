#ifndef _BINTREE__H_
#define  _BINTREE__H_

#include<iostream>
#include<cstring>
#include<queue>
#include<stack>
using namespace std;

template<class T>
struct Node
{
	Node()
	{}

	Node(const T& data)
	:_data(data)
	, _left(NULL)
	, _right(NULL)
	{}
	
	Node<T>* _left;
	Node<T>* _right;
	T _data;
};



template<class T>
class BinTree
{
public:
	typedef Node<T>* pNode;
	typedef Node<T> Node;
	typedef void(*pFun)(pNode);

	BinTree()
		:_pRoot(NULL)
	{}

	BinTree(const BinTree& bt)
		:_pRoot(NULL)
	{
		_pRoot = _CopyBinTree(bt._pRoot);
	}

	~BinTree()
	{
		_CleanBinTree();
	}
	BinTree& operator=(const BinTree& bt)
	{
		if (&bt != this)
		{
			if (this->_pRoot != NULL)
			{
				_CleanBinTree();
			}
			_pRoot = _CopyBinTree(bt._pRoot);
		}
		return *this;
	}

	void CreateBinTree(T* arr, size_t size, T invalid)//创建二叉树
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

	size_t Height()//树的高度,根结点默认为1
	{
		return _DepthBinTree(_pRoot);
	}

	size_t NodeCount()//树的结点的总个数
	{
		return _NodeCount(_pRoot);
	}

	size_t LeafNodeNumber()//叶子节点的个数
	{
		return _LeafNodeNumber(_pRoot);
	}

	size_t NumOfKNode(size_t k)//第k层节点的个数
	{
		return _NumOfKNode(_pRoot, k);
	}

	bool IsHasNode(const pNode node)//判断一个节点是否在二叉树内
	{
		return _IsHasNode(_pRoot, node);
	}

	pNode FindNode(const T elem)//查找值为elem的节点
	{
		return _FindNode(_pRoot, elem);
	}

	void Image()//树的镜像
	{
		_ImageBintree(_pRoot);
	}

	bool IsFullBinTree()//判断是否为完全二叉树
	{
		return _IsFullBinTree(_pRoot);
	}

	void PreVisitNoRec()//非递归前序遍历
	{
		_PreVisitNoRec(_pRoot, _PrintNode);
	}

	void InVisitNoRec()//非递归中序遍历
	{
		_InVisitNoRec(_pRoot,_PrintNode);
	}

	void PostVisitNoRec()//非递归后序遍历
	{
		_PostVisitNoRec(_pRoot,_PrintNode);
	}
protected:
	void _CreateBinTree(pNode& root,T* arr,size_t size,size_t& index,T invalid )
	{			
		if (index < size && invalid != arr[index])
		{
			root = new Node(arr[index]);			
			_CreateBinTree(root->_left, arr, size, ++index, invalid);
			_CreateBinTree(root->_right, arr, size, ++index, invalid);
		}
		else
			root = NULL;
	}
	void _CleanBinTree()
	{
		_PostVisit(_pRoot,_CleanNode);
	}

	pNode _CopyBinTree(const pNode& pd)
	{
		pNode pNewNode = NULL;
		if (pd)
		{
			pNewNode = new Node(pd->_data);
			if (pd->_left)
				pNewNode->_left = _CopyBinTree(pd->_left);
			if (pd->_right)
				pNewNode->_right = _CopyBinTree(pd->_right);
		}
		return pNewNode;
	}
	static void _CleanNode(pNode pd)
	{
		delete pd;
		pd = NULL;
	}
	static void _PrintNode(pNode pd)
	{
		cout << pd->_data <<"->";
	}
	
	void _PreVisit(pNode& pd, pFun visit)
	{
		if (pd)
		{
			visit(pd);
			if (pd->_left)
				_PreVisit(pd->_left, visit);
			if (pd->_right)
				_PreVisit(pd->_right, visit);
		}
	}

	void _InVisit(pNode& pd, pFun visit)
	{
		if (pd)
		{
			if (pd->_left)
				_InVisit(pd->_left, visit);
			visit(pd);
			if (pd->_right)
				_InVisit(pd->_right, visit);
		}
	}

	void _PostVisit(pNode& pd, pFun visit)
	{
		if(pd)
		{
			if (pd->_left)
				_PostVisit(pd->_left, visit);
			if (pd->_right)
				_PostVisit(pd->_right, visit);
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
			if (pCur->_left)
				q.push(pCur->_left);
			if (pCur->_right)
				q.push(pCur->_right);
			q.pop();
		}
	}

	size_t _DepthBinTree(const pNode pd)
	{
		if (NULL == pd)
			return 0;
		if (NULL == pd->_left && NULL == pd->_right)
			return 1;
		size_t LeftDepth = _DepthBinTree(pd->_left);
		size_t RightDepth = _DepthBinTree(pd->_right);

		return (LeftDepth > RightDepth ? LeftDepth + 1 : RightDepth + 1);
	}

	size_t _NodeCount(const pNode pd)
	{
		if (NULL == pd)
			return 0;
		if (NULL == pd->_left && NULL == pd->_right)
			return 1;
		return (1 + _NodeCount(pd->_left) + _NodeCount(pd->_right));
	}

	size_t _LeafNodeNumber(pNode pd)
	{
		if (NULL == pd)
			return 0;
		if (NULL == pd->_left && NULL == pd->_right)
			return 1;
		return (_LeafNodeNumber(pd->_left) + _LeafNodeNumber(pd->_right));
	}

	size_t _NumOfKNode(pNode pd,size_t k)
	{
		if (NULL == pd || 0 == k)
			return 0;
		if (1 == k)
			return 1;
		return (_NumOfKNode(pd->_left, k - 1) + _NumOfKNode(pd->_right, k - 1));
	}

	bool  _IsHasNode(pNode pd, const pNode node)
	{
		if (NULL == pd || NULL == node)
			return false;
		if (pd == node)
			return true;
		if (_IsHasNode(pd->_left, node) || _IsHasNode(pd->_right, node))
			return true;
		return false;
	}

	pNode _FindNode(pNode pd, const T elem)
	{
		if (NULL == pd)
			return NULL;
		if (elem == pd->_data)
			return pd;
		
		if (pd->_left)
			return _FindNode(pd->_left,elem);
		return _FindNode(pd->_right,elem);
	}

	void _ImageBintree(pNode& pd)
	{
		if (NULL == pd)
			return;
		if (pd->_left && pd->_right)
			swap(pd->_left,pd->_right);
		_ImageBintree(pd->_left);
		_ImageBintree(pd->_right);
	}

	bool  _IsFullBinTree(pNode pd)/*将所有节点入队列,包括空节点,
								  如果队列里面有NULl节点,说明不是完全二叉树
								  */
	{
		if (NULL == pd)
			return true;
		queue<pNode> q;
		q.push(pd);
		pNode pCur = pd;
		while (NULL!=pCur)
		{
			
			q.push(pCur->_left);
			q.push(pCur->_right);	
			q.pop();
			if (!q.empty())
				pCur = q.front();
		}
		while (!q.empty())
		{		
			if (NULL != q.front())
				return false;
			q.pop();
		}
		return true;
	}

	void _PreVisitNoRec(pNode pd,pFun visit)
	{
		stack<pNode> s;
		pNode pCur = pd;

		while (!s.empty() || pCur)
		{
			while (pCur)
			{
				visit(pCur);
				s.push(pCur);
				pCur = pCur->_left;
			}
			if (!s.empty())
			{
				pCur = s.top();
				s.pop();
				pCur = pCur->_right;
			}
		}

	}

	void _InVisitNoRec(pNode pd, pFun visit)
	{
		pNode pCur = pd;
		stack<pNode> s;
		while (!s.empty() || pCur)
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_left;
			}
			if (!s.empty())
			{
				pCur = s.top();
				s.pop();
				visit(pCur);
				pCur = pCur->_right;
			}	
		}
	}
	
	void _PostVisitNoRec(pNode pd, pFun visit)
	{
		pNode pCur = pd;
		pNode pLastVisit = NULL;//上次访问的节点
		stack<pNode> s;
		
		while (pCur)
		{
			s.push(pCur);
			pCur = pCur->_left;
		}
		while (!s.empty())
		{
			pCur = s.top();
			s.pop();

			//此时一个节点被访问的前提是不能有右子树或者右子树没有被访问过
			if (NULL == pCur->_right || pLastVisit == pCur->_right)
			{
				visit(pCur);
				pLastVisit = pCur;
			}
			else//以右子树为根继续向下访问
			{
				s.push(pCur);
				pCur = pCur->_right;
				while (pCur)
				{
					s.push(pCur);
					pCur = pCur->_left;
				}
			}
		}	
	}

private:
	pNode _pRoot;
};

void TestBintree()//测试代码
{
	BinTree<char> bt;
	char arr[] = "ABD###CE##F";/*
							      A
								 / \
							    B   C
							   /	/\
						      D    E  F			
							   */
	bt.CreateBinTree(arr,strlen(arr),'#');//创建树
	cout << "前序:" << endl;
	bt.PreVisit();//前序遍历测试
	cout << endl;
	bt.PreVisitNoRec();//非递归前序遍历
	cout << endl;

	cout << "中序:" << endl;
	bt.InVisit();//中序遍历测试
	cout << endl;
	bt.InVisitNoRec();//非递归中序遍历
	cout << endl;

	cout << "后序:" << endl;
	bt.PostVisit();//后序遍历测试
	cout << endl;
	bt.PostVisitNoRec();
	cout << endl;

	cout << "层序:" << endl;
	bt.LevelVisit();//层序遍历测试
	cout << endl;

	BinTree<char> bt1(bt);//拷贝构造函数测试
	bt1.LevelVisit();
	cout << endl;

	BinTree<char> bt3;//赋值运算符重载测试
	bt3 = bt1;
	bt3.LevelVisit();
	cout << endl;

	cout << bt3.Height() << endl;//树的高度

	cout << bt3.NodeCount() << endl;//树的节点个数

	cout << bt3.LeafNodeNumber() << endl;//叶子结点的个数

	cout << bt3.NumOfKNode(3) << endl;//第k层的节点个数

	Node<char>* p = bt3.FindNode('F');//查找值为F的节点

	if (bt3.IsHasNode(p))//判断是否存在某节点
		cout << "存在节点" << endl;
	else
		cout << "不存在节点" << endl;

	bt3.Image();//镜像
	bt3.LevelVisit();//ACBFED
	bt3.Image();

	/*测试是否为完全二叉树*/
	char arr1[] = "ABD##E##C";/*
								 A
								/ \
							   B   C
							  / \
							 D   E     */
	char arr2[] = "ABD##E##C#F";/*
								 A
								/ \
							   B   C
							  / \   \
							 D   E   F  */
	char arr3[] = "ABD#F##E##C";/*
								 A
								/ \
							   B   C
							  / \
							 D   E
								  \
								   F   */
	char arr4[] = "ABD##E##CF";/*
								 A
								/ \
							   B   C
							  / \  /
							 D  E  F    */
	BinTree<char> bt_arr1;//是
	bt_arr1.CreateBinTree(arr1, strlen(arr1), '#');

	BinTree<char> bt_arr2;//不是
	bt_arr2.CreateBinTree(arr2, strlen(arr2), '#');

	BinTree<char> bt_arr3;//不是
	bt_arr3.CreateBinTree(arr3, strlen(arr3), '#');

	BinTree<char> bt_arr4;//是
	bt_arr4.CreateBinTree(arr4, strlen(arr4), '#');

	if (bt_arr1.IsFullBinTree())
		cout << "是完全二叉树" << endl;
	else
		cout << "非完全二叉树" << endl;

	if (bt_arr2.IsFullBinTree())
		cout << "是完全二叉树" << endl;
	else
		cout << "非完全二叉树" << endl;

	if (bt_arr3.IsFullBinTree())
		cout << "是完全二叉树" << endl;
	else
		cout << "非完全二叉树" << endl;

	if (bt_arr4.IsFullBinTree())
		cout << "是完全二叉树" << endl;
	else
		cout << "非完全二叉树" << endl;
	
}

#endif// _BINTREE__H_