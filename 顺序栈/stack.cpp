#include<iostream>
#include<stdlib.h>
#include<assert.h>
using namespace std;



template<typename T>
class Stack
{
public:
	Stack()
	: _array(new T[3])
	, _size(0)
	, _capacity(3)
	{}

	void Push(const T& data);
	void Pop();
	T& Top();
	const T& Top()const;
	size_t Size()const;
	bool Empty()const;

private:
	void _CheckCapacity();
private:
	T* _array;
	size_t _capacity;
	size_t _size;
}; 

template<typename T>
void Stack<T>::_CheckCapacity()
{
	if(_size >= _capacity)
	{
		size_t NewSize = _capacity*2 + 1;
		T* temp = new T[NewSize];
		memcpy(temp,_array,_size*sizeof(T));
		_capacity = NewSize;
	}
}

template<typename T>
void Stack<T>::Push(const T& data)
{
	_CheckCapacity();
	*(_array+_size) = data;
	++_size;
}

template<typename T>
void Stack<T>::Pop()
{
	if(_size > 0)
		--_size;
}

template<typename T>
T& Stack<T>::Top()
{
	return *(_array+_size-1);
}

template<typename T>
const T& Stack<T>::Top()const
{
	if(_size > 0)
		return *(_array+_size-1)
}

template<typename T>
size_t Stack<T>::Size()const
{
	return _size;
}

template<typename T>
bool Stack<T>::Empty()const
{
	return (_size == 0);
}

void test_Satck()
{
	Stack<int> s;
	s.Push(1);
	s.Push(2);
	s.Push(3);
	s.Push(4);
	int a = s.Top();//4
	s.Pop();
	a = s.Top();//3
	s.Pop();

	size_t sz = s.Size();//2
	s.Pop();
	s.Pop();
	if(s.Empty())
		cout<<"kong"<<endl;
}

bool IsBrackets(char ch)
{
	return (ch=='(' || ch==')'
		||ch=='{' || ch=='}'
		||ch=='[' || ch==']');
}
bool CheckBrackets(char* str,int sz)
{
	assert(str != NULL && sz >= 0);
	Stack<char> s;
	for(int i=0; i<sz; ++i)
	{
		if(!IsBrackets(str[i]))
			continue;	
	
		else if(str[i]=='(' || str[i]=='{' || str[i]=='[')
		{
			s.Push(str[i]);
		}
		else if(str[i]==')'|| str[i]==']' || str[i]=='}')
		{
			if(s.Empty())
			{
				cout<<"右括号多"<<endl;
				return false;
			}
			else if((s.Top()=='('&&str[i]==')')
				|| (s.Top()=='{'&&str[i]=='}')
				|| (s.Top()=='['&&str[i]==']'))
			{
				s.Pop();
			}
			else /*if((s.Top()=='('&&str[i]!=')')
				|| (s.Top()=='('&&str[i]!=')')
				|| (s.Top()=='('&&str[i]!=')'))*/
			{
				cout<<"括号次序不匹配"<<endl;
				return false;
			}
		}
	}

	if(!s.Empty())
	{
		cout<<"左括号多了"<<endl;
		return false;
	}
	else
	{
		cout<<"括号匹配"<<endl;
		return true;
	}

}

void test_CheckBrackets()
{
	char* str1 = "((aaa){}[])aaa";//ok
	char* str2 = "(((){[]})";//左多有少
	char* str3 = "{()}[]}";//右多左少
	char* str4 = "{[(})]";//左右不匹配
	CheckBrackets(str2,strlen(str2));
}


