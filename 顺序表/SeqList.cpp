#include<iostream>
#include<stdlib.h>
#include<assert.h>

#define datatype int

using namespace std;


class SeqList
{
public:
	friend ostream& operator<<(ostream& os,SeqList& s);
	SeqList()
		:_str(new datatype[3])
		,_size(0)
		,_capacity(3)
	{}
	SeqList(datatype *arr,int size)
		:_str(new datatype[size])
		,_size(size)
		,_capacity(size)
	{
		memcpy(_str,arr,size*sizeof(datatype));
	}
	SeqList(const SeqList& s)
	{
		_str = new datatype[s._capacity];
		memcpy(_str,s._str,s._size*sizeof(datatype));
		_size = s._size;
		_capacity = s._capacity;
	}
	SeqList& operator=(const SeqList& s)
	{
		if(this != &s)
		{
			delete[] _str;
			//_str(s._str);
			memcpy(_str,s._str,_size*sizeof(datatype));
			_size = s._size;
			_capacity = s._capacity;
		}
		return *this;
	}
	datatype& operator[](int pos)
	{
		if(pos>0 && pos<_size)
		{
			return _str[pos];
		}
	}
	~SeqList()
	{
		if(_str != NULL)
			delete[] _str;
		_size = 0;
		_capacity = 0;
	}
	void PushBack(const datatype& elem)//尾插
	{
		assert(_str);
		CheckCapacity();
		_str[_size] = elem;
		_size++;
	}
	void PopBack()
	{
		assert(_str);
		if(_size > 0)
			_size--;
	}
	void PushFront(const datatype& elem)
	{
		assert(_str);
		CheckCapacity();
		for(int i=_size; i>0; i--)
		{
			_str[i] = _str[i-1];
		}
		_str[0] = elem;
		_size++;
	}
	void PopFront()
	{
		assert(_str);
		if(_size > 0)
		{
			for(int i=0; i<_size-1; i++)
			{
				_str[i] = _str[i+1];
			}
			_size--;
		}	
	}
	void CheckCapacity()
	{
		if(_size >= _capacity)
		{
			datatype *tmp = new datatype[_capacity+10];
			memcpy(tmp,_str,_size*sizeof(datatype));
			delete[] _str;
			_str = tmp;
			_capacity += 10;
		}
	}

	void Insert(int pos,const datatype& elem)
	{
		assert(_str);
		if(pos<0 || pos>_size)
			return;	
		CheckCapacity();
		for(int i=_size; i>pos; i--)
		{
			_str[i] = _str[i-1];
		}
		_str[pos] = elem;
		_str++;
	}

	void Erase(int pos)
	{
		assert(_str);
		if(pos<0 || pos>=_size)
			return;
		for(int i=pos; i<_size-1; i++)
		{
			_str[i] = _str[i+1];
		}
		_size--;
	}

	// 获取顺序表中第一个元素
	datatype& Front()
	{
		return _str[0];
	}
	const datatype& Front()const
	{
		return _str[0];
	}
	// 获取顺序表中最后一个元素
	datatype& Back()
	{
		return _str[_size-1];
	}
	const datatype& Back()const
	{
		return _str[_size-1];
	}

	void print()
	{
		for(int i=0; i<_size; i++)
		{
			cout<<" "<<_str[i];
		}
	}
private:
	datatype *_str;
	int _size;
	int _capacity;

};

ostream& operator<<(ostream& os,SeqList& s)
{
	for(int i=0; i<s._size; i++)
	{
		os<<s._str[i]<<" ";
	}
	return os;
}

int main()
{
	int arr[] = {1,2,3,4};
	SeqList s1(arr,4);
	s1.print();
	cout<<s1;
	return 0;
}
