
#include<iostream>
using namespace std;

template<typename T>
struct  BinarySearchNode
{
	BinarySearchNode* _pLeft;
	BinarySearchNode* _pRight;
	T _data;
	BinarySearchNode(const T& data=T())
		:_pLeft(NULL)
		, _pRight(NULL)
		, _data(data)
	{}
};

template<typename T>
class  BinarySerachTree
{
	typedef BinarySearchNode<T> Node;
	typedef Node* pNode;
public:
	BinarySerachTree()
		:_pRoot(NULL)
	{}

	bool Insert(const T& data)
	{
		if (NULL == _pRoot)
		{
			_pRoot = new Node(data);
			return true;
		}
		pNode pCur = _pRoot;
		pNode pParent = pCur;

		while (pCur)
		{
			if (pCur->_data > data)
			{
				pParent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (pCur->_data < data)
			{
				pParent = pCur;
				pCur = pCur->_pRight;
			}
			else
				return false;
		}
		if (pParent->_data < data)
			pParent->_pRight = new Node(data);
		else
			pParent->_pLeft = new Node(data);
			
		return true;
	}
	
	pNode Find(const T& data)
	{
		if (NULL == _pRoot)
			return NULL;

		pNode pCur = _pRoot;
		while (pCur)
		{
			if (data == pCur->_data)
				return pCur;
			else if (pCur->_data < data)
				pCur = pCur->_pRight;
			else
				pCUr = pCur->_pLeft;
		}
		return pCur;
	}

	bool Delete(const T& data)
	{
		if (NULL == _pRoot)
			return false;

		if (NULL == _pRoot->_pLeft && NULL == _pRoot->_pRight)//ֻ��һ���ڵ�����
		{
			if (data == _pRoot->_data)
			{
				delete _pRoot;
				_pRoot = NULL;
				return true;
			}
			else
				return false;
		}

		pNode pCur = _pRoot;
		pNode pDel = NULL;
		pNode pParent = pCur;
		

		while (pCur)
		{
			if (data == pCur->_data)
			{
				break;
			}
			else if (pCur->_data < data)
			{
				pParent = pCur;
				pCur = pCur->_pRight;
			}
			else
			{
				pParent = pCur;
				pCur = pCur->_pLeft;
			}
		}
		if (NULL == pCur)
			return false;

		if (NULL == pCur->_pLeft)//Ҫɾ���ڵ����Ϊ�ջ�������������Ϊ��
		{
			if (pCur != _pRoot)
			{
				if (pParent->_pLeft == pCur)//Ҫɾ���ڵ�λ����һ�ڵ�����
				{
					pParent->_pLeft = pCur->_pRight;
				}
				else if (pParent->_pRight == pCur)
				{
					pParent->_pRight = pCur->_pRight;
				}
			}
			else
				_pRoot = pCur->_pRight;
			
		}
		else if (NULL == pCur->_pRight)//��ɾ�ڵ���ұ�Ϊ��
		{
			if (pCur != _pRoot)
			{
				if (pParent->_pLeft == pCur)
				{
					pParent->_pLeft = pCur->_pLeft;
				}
				else if (pParent->_pRight == pCur)
				{
					pParent->_pRight = pCur->_pLeft;
				}
			}
			else
				_pRoot = pCur->_pLeft;
		}
		else
			//��ɾ�ڵ��������������Ϊ��,
			//�ҵ������������ұ߽ڵ��滻��ɾ�ڵ�,�����ҵ���������������滻��ɾ�ڵ�
			//Ȼ��ɾ���ҵ��Ľڵ�
		{
			pNode pDel = pCur->_pRight;//�������ĸ����
			pParent = pCur;
			while (pDel->_pLeft)
			{
				pParent = pDel;
				pDel = pDel->_pLeft;
			}
			pCur->_data = pDel->_data;
			pCur = pDel;

			if (pDel == pParent->_pLeft)
				pParent->_pLeft = pDel->_pRight;
			else if (pDel == pParent->_pRight)
				pParent->_pRight = pDel->_pRight;
		}
		delete pCur;
		pCur = NULL;
		return true;
	}

	void InOder()
	{
		if (_pRoot == NULL)
			return;
		cout << "�������:";
		_InOder(_pRoot);
		cout << endl;
	}
private:
	void _InOder(pNode pRoot)
	{
		if (pRoot == NULL)
			return;
		_InOder(pRoot->_pLeft);
		cout << pRoot->_data << " ";
		_InOder(pRoot->_pRight);
	}
private:
	pNode _pRoot;
};

void BinarySearchTreeTest()
{
	BinarySerachTree<int> bs;
	//int arr[] = {5,7,8,9,6,3,4,1,0,2};
	int arr[] = {5,7,6,8};
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		bs.Insert(arr[i]);
	}
	bs.InOder();
	//bs.Delete(5);
	//bs.Delete(7);
	bs.Delete(5);
	bs.InOder();
}
