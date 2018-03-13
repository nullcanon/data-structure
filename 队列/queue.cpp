#include<iostream>

using namespace std;

#if 0
template<class T,size_t N = 8>//基于数组实现
class queue
{
public:
	queue()
		:_rear(0)
		, _front(0)
		, _count(0)
	{}

	void push(const T data)
	{
		if (_count == N)
			return;
		_arr[_rear] = data;
		_rear = (_rear + 1) % N;
		++_count;
	}
	void pop()
	{
		if (empty())
			return;
		_front = (_front + 1) % N;	
		--_count;
	}

	bool empty()
	{
		return _count == 0;
	}

	T GetFront()
	{
		return _arr[_front];
	}
	
	size_t size()
	{
		return _count;
	}

	void print()
	{
		if (!empty())
		{
			for (size_t i = 0; i < _count; ++i)
			{
				cout << _arr[i] << " ";
			}
			cout << endl;
		}
	}
private:
	T _rear;
	T _front;
	T _arr[N];
	size_t _count;
};

int main()
{
	queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.print();
	q.push(4);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.print();

	return 0;
}
#endif
template<class T>//基于节点实现
struct Node
{
public:
	Node<T>(const T& data)
		: pNext(NULL)
		, elem(data)
	{}

	Node<T>* pNext;
	T elem;
};

template<class T>
class queue
{
	
public:
	queue()
		:_rear(NULL)
		, _front(NULL)
	{}
	~queue()
	{
		clear();
	}
	void push(const T data)
	{
		if (_rear == NULL && _front == NULL)
		{
			_rear = new Node<T>(data);
			_front = _rear;
		}
		else
		{
			_rear->pNext = new Node<T>(data);
			_rear = _rear->pNext;
		}
	}

	void pop()
	{
		if (!empty())
		{
			if (_front == _rear)
			{
				delete _front;
				_front = _rear = NULL;
			}
			else
			{
				Node<T>* pDel = _front;
				_front = _front->pNext;
				delete pDel;
			}
		}
	}

	bool empty()
	{
		return _front == NULL;
	}

	T GetFront()
	{
		return _front->elem;
	}
	
	T GetRear()
	{
		return _rear->elem;
	}

	int size()
	{
		Node<T>* pCur = _front;
		int count = 0;
		while (pCur != _rear)
		{
			++count;
			pCur = pCur->pNext;
		}
		return (++count);
	}

	void print()
	{
		Node<T>* pCur = _front;
		while (pCur != _rear)
		{
			cout << pCur->elem << " ";
			pCur = pCur->pNext;
		}
		cout << _rear->elem << endl;
	}

	void clear()
	{
		Node<T>* pCur = _front;
		while (pCur != _rear)
		{
			Node<T>* pDel = pCur;
			pCur = pCur->pNext;
			delete pDel;		
		}
		delete _rear;
		_front = _rear = NULL;
	}
private:
	Node<T>* _front;
	Node<T>* _rear;
};

int main()
{
	queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.print();
	q.push(4);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.push(5);
	q.pop();
	q.print();
	cout << q.size() << endl;
	cout << q.GetFront() << endl;
	cout << q.GetRear() << endl;
	q.clear();

	return 0;
}