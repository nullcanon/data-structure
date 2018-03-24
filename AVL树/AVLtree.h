
#include<iostream>
#include<assert.h>
using namespace std;

template<typename K,typename V>
struct AVLtreeNode
{
	K _key;
	V _value;
	int _bf;//ƽ������
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
		if (key > pParent->_key)//���뵽parent���ұ�
		{
			pParent->_pRight = pCur;
			pParent->_bf++;
		}			
		else if (key < pParent->_key)//���뵽parent�����
		{
			pParent->_pLeft = pCur;
			pParent->_bf--;
		}
			
		pCur->_pParent = pParent;

		//�Ӳ���ڵ㵽��������,����ƽ������
		//���ĳ��������ƽ��,��תʹ֮ƽ��
		while (pParent)
		{
			if (0 == pParent->_bf)//���ĸ߶�û��,ֱ�ӷ���
				break;
			else if (1 == pParent->_bf || -1 == pParent->_bf)//��ǰ����Ҫ��ת,�������ϲ���
			{
				pNode ppParent = pParent->_pParent;
				if (ppParent != NULL)
				{
					if (pParent == ppParent->_pLeft)//����ppParent��������������
						ppParent->_bf--;
					else
						ppParent->_bf++;
				}
				pParent = ppParent;//���������ж�
			}
			else//����Ҫ��ת,���������
			{
				//parent���������,2��-2
				if (2 == pParent->_bf)
				{
					//��������������������
					//���subRΪ1����,subRΪ-1������
					if (1 == pParent->_pRight->_bf)
						_RotateL(pParent);
					else
						_RotateRL(pParent);
				}
				else if (-2 == pParent->_bf)
				{
					//�ҵ�������������������
					//���subLΪ-1�ҵ���,subLΪ1������
					if (-1 == pParent->_pLeft->_bf)
						_RotateR(pParent);
					else
						_RotateLR(pParent);
				}
				break;//��ת֮�����µĸ��ڵ��ƽ������Ϊ0,����Ҫ�����ϸ���
			}
		}
		return true;
	}

	void Inder()
	{
		if (NULL == _pRoot)
			return;
		cout << "�������:";
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

	void _RotateL(pNode &pParent)//����
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
			//���pParent������˫�׵�����,����˫�׵���ָpsubR
			if (pParent == pParent->_pParent->_pLeft)
				pParent->_pParent->_pLeft = psubR;
			else
				pParent->_pParent->_pRight = psubR;
		}
		else
			_pRoot = psubR;

		pParent->_pParent = psubR;
		pParent->_bf = psubR->_bf = 0;///����ƽ������
	}

	void _RotateR(pNode &pParent)//�ҵ���
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
			//���pParent����˫�׵�����,����˫�׵���ָ��psubL
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

	void _RotateRL(pNode &pParent)//������
	{
		pNode psubR = pParent->_pRight;
		pNode psubRL = psubR->_pLeft;

		//����psubRL��bf���жϽڵ�����λ��
		//�Ӷ�ȷ���ڵ���ת������ڵ��bf
		int bf = psubRL->_bf;

		_RotateR(pParent->_pRight);
		_RotateL(pParent);

		if (0 == bf)//Ҫ����ľ���subRL����
		{
			psubR->_bf = pParent->_bf = 0;
		}
		else if(1 == bf)//�ڵ������psuRL���ұ�
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
	void _RotateLR(pNode &pParent)//������
	{
		pNode psubL = pParent->_pLeft;
		pNode psubLR = psubL->_pRight;
		int bf = psubLR->_bf;

		_RotateL(pParent->_pLeft);
		_RotateR(pParent);

		if (0 == bf)//Ҫ����ľ���subLR����
		{
			psubL->_bf = pParent->_bf = 0;
		}
		else if (1 == bf)//�ڵ������psuLR���ұ�
		{
			psubL->_bf = -1;
			pParent->_bf = 0;
		}
		else//�ڵ������subLR�����
		{
			psubLR->_bf = 0;
			pParent->_bf = 1;
		}
		psubLR->_bf = 0;
	}
private:
	pNode _pRoot;
};

void TestAVLTree1()//����������
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
		cout << "��ƽ��" << endl;
}

void TestAVLTree2()//����������
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
		cout << "��ƽ��" << endl;
}
