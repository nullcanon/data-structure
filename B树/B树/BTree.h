#pragma once

#include<utility> //pair
#include<iostream>
using namespace std;

template<typename K,int M>
struct BTreeNode
{
	K _keys[M];//��ʾ�ؼ������飬M�Ǻ��ӵĸ������࿪��һ�����ڴ���
	BTreeNode<K, M>* _subs[M + 1];//����ָ������
	BTreeNode<K, M>* _parent;//ָ���׵�ָ��
	int _size;//�ؼ��ֵĸ���

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

	// ����һ�����ݣ�
	//����ҵ��ˣ�frist��ʾ�ڵ㣬second��ʾ�ڽ���е�λ��
	//���û�ҵ���first��ʾ������Ӧ�������нڵ㣬secondΪ-1
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
					return pair<pNode, int>(pCur, i);//�Ѿ�����
			}
			pParent = pCur;
			pCur = pCur->_subs[i];
		}
		return pair<pNode, int>(pParent, -1);
	}

	bool Insert(const K& key)
	{
		//����ǿ���
		if (NULL == _pRoot)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size++;
			_pRoot->_parent = NULL;
		}
		//������ǿ�������key�Ƿ���ڣ��ظ�Ĭ�ϲ���ʧ��
		pair<pNode, int> ret = Find(key);
		if (ret.second != -1)//�Ѿ�����
			return false;
		pNode pCur = ret.first;
		pNode pSub = NULL;
		K newKey = key;
		while (true)
		{
			_InsertKey(pCur, newKey, pSub);//���ؼ�ֵ����
			//���_size�Ѿ�����M-1,����Ҫ���з���
			//���ѹ����м�Ԫ�����Ƶ�˫�׽ڵ㣬���ҵ�Ԫ����Ϊ�����µĽڵ�
			//			���˫�׽ڵ����M����Ҫ����˫�׽ڵ�
			if (pCur->_size == M)
			{
				pNode newNode = new Node;
				int mid = pCur->_size >> 1;
				int j = 0, i = mid + 1;
				//���м�Ԫ���Ժ��Ԫ�غͺ��Ӱ��Ƶ��½ڵ�
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
				//�������һ���Һ���
				newNode->_subs[j] = pCur->_subs[i];
				if (newNode->_subs[j])
					newNode->_subs[j]->_parent = newNode;
				pCur->_subs[i] = NULL;

				newKey = pCur->_keys[mid];
				pCur->_keys[mid] = K();
				pSub = newNode;
				pCur->_size = mid;

				//������ѵ��Ǹ����
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
		//�ж�Ҫɾ�����ǲ���Ҷ�ӽڵ㣬����������佻����Ҷ����
		//�����ɾ�ڵ����������Ϊ�գ����ɾ�ڵ�Ϊ��Ҷ�ӽڵ�
		if (pCur->_subs[pos + 1] != NULL)
		{
			//�ҵ�����������Сֵ�����滻
			pNode pMinKey = pCur->_subs[pos + 1];
			while (pMinKey->_subs[0])
				pMinKey = pMinKey->_subs[0];
			pCur->_keys[pos] = pMinKey->_keys[0];
			pCur = pMinKey;
			_MoveLeft(pCur, 0);//ɾ���������pCur
		}
		else
			_MoveLeft(pCur, pos);

		//�ж��Ƿ�����B���������������
		//����ؼ�����������M/2-1��ȡ��
		int mid = ((M + 1) >> 1) - 1;
		while (true)
		{
			//���ؼ���ĸ���С������ֵ������Ҫ���е���
			if (pCur->_size < mid)
			{
				if (pCur == _pRoot)
					break;
				pNode pParent = pCur->_parent;
				pos = 0;

				//�ҵ���ǰ�ڵ��ڸ��ڵ��е�λ��
				while (pParent->_subs[pos] != pCur && pos < pParent->_size)
					++pos;
				if (pos == 0)//��ǰ�ڵ��������׵���ߣ�������ҽ����е���
					_LeftAdjust(pParent, pCur, mid, pos);
				else
					_RightAdjust(pParent, pCur, mid, pos-1);
				pCur = pParent;
			}
			else
				break;
		}
		//���������������0����ɾ������㣬�����ĺ��ӵ�����
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
		//�����ǰ�ڵ���ҽ��Ĺؼ�����ڹؼ������ޣ���������
		pNode pRight = pParent->_subs[pos + 1];
		if (pRight->_size > mid)
		{
			//�����ڵ��иպô��ڱ�ɾ���ؼ���Ĺؼ������Ƶ���ɾ��λ��
			pCur->_size++;
			pCur->_keys[pCur->_size - 1] = pParent->_keys[pos];
		
			//�����ֵ�����С�Ĺؼ������Ƶ����ڵ���k��λ��
			pParent->_keys[pos] = pRight->_keys[0];

			//�����ֵ��е�����������ָ��ƽ�Ƶ���ɾ���ؼ������ڽ���е����λ��
			pCur->_subs[pCur->_size] = pRight->_subs[0];
			if (pRight->_subs[0])
				pRight->_subs[0]->_parent = pCur;
			pRight->_subs[0] = pRight->_subs[1];
			//�����ֵܵĽ�����ٽ������ߵĹؼ����ָ��λ����ʣ��Ĺؼ����ָ�����������
			_MoveLeft(pRight, 0);
		}
		else//ͨ���ϲ����
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
		//�Ѹ��ڵ���Ӧ��ֵ�Ĺؼ����Ƶ�������
		pCur->_keys[i] = pParent->_keys[pos];
		//�����ֵ�����ߵĺ����ƹ���
		pCur->_subs[i + 1] = pBrother->_subs[0];
		if (pBrother->_subs[0])
			pBrother->_subs[0]->_parent = pCur;
		i++;
		pCur->_size++;
		//���ֵܽڵ�ָ�뿽������
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
		//�ο�������������
		//ֻ����B����Ҫѭ������ڵ������ֵ
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
		while (i >= 0)//��key���뵽pCur�ڵ���
		{
			if (key < pCur->_keys[i])//�Ӻ���ǰ����������pCur�ڵ��ֵ�ͺ���
			{
				pCur->_keys[i + 1] = pCur->_keys[i];
				pCur->_subs[i + 2] = pCur->_subs[i + 1];
				--i;
			}
			else
				break;
		}
		//��key���뵽��Ӧλ��
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