#pragma once

#include <vector>

//ElemType elements must have operator > and <!
template <typename ElemType>
class MinHeap
{
public:
	MinHeap();
	void Push(const ElemType&);
	ElemType Pop();
	const ElemType& Peek() const;
	void Heapify(const std::vector<ElemType>&);
	size_t GetSize() const;

private:
	std::vector<ElemType> tree;

	void SiftUp(int);
	void SiftDown(int);
	int GetParentInd(int);
	int GetLeftChildInd(int);
	int GetRightChildInd(int);
};

template<typename ElemType>
inline MinHeap<ElemType>::MinHeap()
{	
	//Real elements are starting from index 1 because of easier index management
	tree.push_back(ElemType());
}

template<typename ElemType>
void MinHeap<ElemType>::Push(const ElemType &element)
{
	tree.push_back(element);
	SiftUp(tree.size() - 1);
}

//O(logn)
template<typename ElemType>
ElemType MinHeap<ElemType>::Pop()
{
	if (tree.size() == 1)
	{
		throw "No tree elements!";
	}

	ElemType returnValue = tree[1];
	tree[1] = tree[tree.size() - 1];
	tree.pop_back();
	SiftDown(1);

	return returnValue;
}

template<typename ElemType>
const ElemType& MinHeap<ElemType>::Peek() const
{
	return tree[1];
}

//O(n)
template<typename ElemType>
void MinHeap<ElemType>::Heapify(const std::vector<ElemType>& elements)
{
	tree.clear();
	tree.reserve(elements.size() + 1);
	tree.push_back(0);

	//O(n)
	for (size_t i = 0; i < elements.size(); ++i)
	{
		tree.push_back(elements[i]);
	}

	//O(n)
	//amount / 2 gives us the first node with atleast one child
	for (size_t i = tree.size() / 2; i >= 1; --i)
	{
		SiftDown(i);
	}
}


template<typename ElemType>
size_t MinHeap<ElemType>::GetSize() const
{
	return tree.size() - 1;
}

//O(logn)
template<typename ElemType>
void MinHeap<ElemType>::SiftUp(int index)
{
	int parent = GetParentInd(index);
	if (index == 1 || tree[parent] < tree[index])
	{
		return;
	}

	std::swap(tree[index], tree[parent]);
	SiftUp(parent);
}

//O(logn)
template<typename ElemType>
void MinHeap<ElemType>::SiftDown(int index)
{
	int leftChild = GetLeftChildInd(index);
	int rightChild = GetRightChildInd(index);
	if (index >= tree.size() || leftChild >= tree.size())
	{
		return;
	}

	int minIndex = leftChild;
	if (rightChild < tree.size() && tree[minIndex] > tree[rightChild])
	{
		minIndex = rightChild;
	}

	if (tree[minIndex] < tree[index])
	{
		std::swap(tree[minIndex], tree[index]);
		SiftDown(minIndex);
	}
}

template<typename ElemType>
int MinHeap<ElemType>::GetParentInd(int index)
{
	return index >> 1;
}

template<typename ElemType>
int MinHeap<ElemType>::GetLeftChildInd(int index)
{
	return index << 1;
}

template<typename ElemType>
int MinHeap<ElemType>::GetRightChildInd(int index)
{
	return (index << 1) | 1;
}
