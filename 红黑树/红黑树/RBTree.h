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

			//����pλ��g�ķ���
			if (pParent == pGrdftr->_pLeft)
			{
				//�����Ƿ����,�ڵ����ɫ
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
					//����cur��p���ı�
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
				//�����Ƿ����,�ڵ����ɫ
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
					//����cur��p���ı�
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

		//��ͳ�Ƴ����һ·���ĺ�ɫ�ڵ�ĸ���
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
		
		//Ѱ��Ҫɾ���Ľڵ�
		while (pCur)
		{
			if (key == pCur->_v.first)
				break;
			else if (key < pCur->_v.first)
				pCur = pCur->_pRight;
			else
				pCur = pCur->_pLeft;
		}

		//û�ҵ�Ҫɾ�ڵ�
		if (NULL == pCur)
			return false;
		//���Ҫɾ���ڵ����������ӽڵ�,ת��Ҫɾ���ڵ�
		if (pCur->_pLeft != NULL && pCur->_pRight != NULL)
		{
			pNode pRightMin = pCur->_pRight;//�ҵ�����������С�ڵ�
			//��������������߽ڵ�
			while (pRightMin->_pLeft)
				pRightMin = pRightMin->_pLeft;
			pCur->_v = pRightMin->_v;
			pCur = pRightMin;//��pCurָ��Ҫɾ�ڵ�
		}
		pParent = pCur->_pParent;
		pDel = pCur;

		//Ҫɾ���ڵ�����Ϊ��
		//���߼�������ǰ�ȴ���ø��ڵ��ָ��
		//�ں���Ĳ�����ֱ��ɾ������
		if (NULL == pDel->_pLeft)
		{
			//���Ҫɾ���Ǹ����
			if (_pRoot == pDel)
			{
				_pRoot = pDel->_pRight;
				//����Һ��Ӵ���,����һ��Ϊ��ɫ
				if (NULL != pDel->_pRight)
				{
					_pRoot->_pParent = NULL;
					_pRoot->_color = BLACK;
				}
				delete pDel;
				return true;
			}

			//�ñ�ɾ�ڵ�ĸ��ڵ�ָ��ɾ�ڵ�ĺ��ӽڵ�
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
			//���Ҫɾ���Ǹ����
			if (_pRoot == pDel)
				_pRoot = pDel->_pLeft;
			//������Ӵ���,����һ��Ϊ��ɫ
			if (NULL != pDel->_pLeft)
			{
				_pRoot->_pParent = NULL;
				_pRoot->_color = BLACK;
			}
			delete pDel;
			return true;

			//�ñ�ɾ�ڵ�ĸ��ڵ�ָ��ɾ�ڵ�ĺ��ӽڵ�
			if (pParent->_pLeft == pDel)
				pParent->_pLeft = pDel->_pRight;
			else
				pParent->_pRight = pDel->_pRight;
			if (pDel->_pRight)
				pDel->_pRight->_pParent = pParent;
			pCur = pDel->_pLeft;//��pCur��Ǻ��ӽڵ�
		}

		//���Ҫɾ���ڵ��Ǻ�ɫ,ֱ��ɾ��
		if (pDel->_color == RED)
		{
			delete pDel;
			return true;
		}

		//�����ɾ�ڵ�Ϊ�ڵ�,���亢���Ǻ��,�����Ӹ�Ϊ��ɫ����
		if (BLACK == pDel->_color && pCur && RED == pCur->_color)
		{
			pCur->_color = BLACK;
			delete pDel;
			return true;
		}

		//Ҫɾ���ڵ��Ǻ�ɫ��
		while (pParent)
		{
			//Ҫɾ���ڵ��Ǹ��׵�����
			if (pParent->_pLeft == pDel)
			{
				pNode pSubR = pParent->_pRight;//parent���Һ���,һ������

				//�Һ����Ǻ�ɫ��,��Ӧ�����,ע�������pSubR��ͼƬ�б�ʾΪS
				if (pSubR->_color == RED)
				{
					_RotateLeft(pParent);
					pSubR->_color = BLACK;
					pParent->_color = RED;
				}
				else//pSubRΪ��ɫ
				{
					pNode pSubRL = pSubR->_pLeft;
					pNode pSubRR = pSubR->_pRight;

					//parentΪ��ɫ
					//(SubR˫�ڵ�Ϊ��)����(SubR��˫�ڵ㲢��˫�ڵ�Ϊ��)
					//��Ӧ�����
					if (BLACK == pParent->_color && (NULL == pSubR->_pLeft && NULL == pSubR->_pRight)
						|| (pSubRL && pSubRR && BLACK == pSubRL->_color && BLACK == pSubRR->_color))
					{
						pSubR->_color = RED;
						pCur = pParent;
						pParent = pCur->_pParent;
					}
					else
					{
						if (RED == pParent->_color)//���ڵ�Ϊ��ɫ
						{
							//���SubR�ĺ�����Ϊ�� ����û�к�����
							//��Ӧ�����
							if ((NULL == pSubRL && NULL == pSubRR) ||
								(pSubRL && pSubRR && BLACK == pSubRL->_color && BLACK == pSubRR->_color))
							{
								pParent->_color = BLACK;
								pSubR->_color = RED;
								break;
							}
						}

						//��Ӧ�����
						if (RED == pSubRL->_color)
						{
							_RotateRight(pSubR);
							pSubR = pSubRL;
						}

						//��Ӧ�����
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
			else//Ҫɾ���ڵ��Ǹ��ڵ��Һ��ӵ����,�������෴
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
						if (RED == pParent->_color)//���ڵ�Ϊ��ɫ
						{
							if ((NULL == pSubLL && NULL == pSubLR) ||
								(pSubLL && pSubLR && BLACK == pSubLL->_color && BLACK == pSubLR->_color))
							{
								pParent->_color = BLACK;
								pSubL->_color = RED;
								break;
							}
						}

						//��Ӧ�����
						if (RED == pSubLL->_color)
						{
							_RotateLeft(pSubL);
							pSubL = pSubLR;
						}

						//��Ӧ�����
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
		cout << "�Ǻ����" << endl;
	else
		cout << "�Ǻ����" << endl;
}