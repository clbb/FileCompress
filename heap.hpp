#pragma once
#pragma once 
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<assert.h>
using namespace std;

template<class T>
struct Less
{
	bool operator() (const T & l, const T & r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator() (const T & l, const T & r)
	{
		return l > r;
	}
};

template<class T, class Compare = Less<T>>
class Heap {
private:
	vector<T> _array;
protected:
	void Adjustdown(int root)
	{
		size_t child = root * 2 + 1;
		while (child < _array.size())
		{
			if (child + 1 < _array.size() && Compare()(_array[child + 1], _array[child]))
			{
				child++;
			}
			if (Compare()(_array[child], _array[root]))
			{
				swap(_array[child], _array[root]);
				root = child;
				child = root * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void Adjustup(int child)
	{
		int root = (child - 1) / 2;
		while (root >= 0)
		{
			if (Compare() (_array[child], _array[root]))
			{
				swap(_array[child], _array[root]);
				child = root;
				root = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
public:
	Heap()
	{}
	Heap(T *array, int size)
	{
		int i;
		for (i = 0; i < size; i++)
		{
			_array.push_back(array[i]);
		}
		for (i = (_array.size() - 2) / 2; i >= 0; i--)
		{
			Adjustdown(i);
		}
	}
	void push(T data)
	{
		_array.push_back(data);
		Adjustup(_array.size() - 1);
	}
	void pop()  //PopµÄÊÇ¸ù
	{
		swap(_array[0], _array[_array.size() - 1]);
		_array.pop_back();
		Adjustdown(0);
	}
	T & top()
	{
		assert(_array.size() > 0);
		return _array[0];
	}
	size_t size()
	{
		return _array.size();
	}
};


