#pragma once
#pragma once 
#include<iostream>
#include"heap.hpp"
using namespace std;

template<class T>
struct HuffmanTreeNode {
	HuffmanTreeNode<T> *_left;
	HuffmanTreeNode<T> *_right;
	HuffmanTreeNode<T> *_parent;
	T _weight;

	HuffmanTreeNode(const T weight)
		:_left(NULL)
		, _right(NULL)
		, _weight(weight)
		, _parent(NULL)
	{}
};

template<class T>
struct NodeCompare
{
	bool operator()(HuffmanTreeNode<T> *l, HuffmanTreeNode<T> *r)
	{
		return l->_weight < r->_weight;
	}
};

template<class T, class Compare>
class HuffmanTree {
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	void Create(const T *a, size_t size, T invalid)
	{
		assert(a);
		Heap<Node*, NodeCompare<T>> minHeap;
		for (size_t i = 0; i < size; i++)
		{
			if (a[i] != invalid)
			{
				Node *newnode = new Node(a[i]);
				minHeap.push(newnode);
			}
		}
		while (minHeap.size() >= 2)
		{
			Node *left = minHeap.top();
			minHeap.pop();
			Node *right = minHeap.top();
			minHeap.pop();

			Node *parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			minHeap.push(parent);
		}
		_root = minHeap.top();
		minHeap.pop();
	}
	Node *GetRoot()
	{
		return _root;
	}
private:
	Node *_root;
};
