#include "pch.h"
#include "Container.h"

namespace Containers
{
	TEM Container<T, Alloc>::Container() : Alloc_{}, ControlBlockAlloc_{}, Count_{}
	{
		FakeRoot_ = CreateNode();
		EndControlBlock_ = CreateControlBlock();
		EndControlBlock_->Stack_.push(FakeRoot_);
	}

	TEM Container<T, Alloc>::~Container()
	{
		Clear();
	}

	TEM TEM_U std::pair<typename Container<T, Alloc>::iterator, bool> Container<T, Alloc>::Insert(U&& value)
	{
		std::stack<Node*> st;
		st.push(FakeRoot_);

		return std::make_pair(iterator(*this, std::move(st)), true);
	}

	TEM TEM_U std::pair<typename Container<T, Alloc>::iterator, bool> Container<T, Alloc>::Erase(U&& value)
	{
		std::stack<Node*> st;
		st.push(FakeRoot_);

		return std::make_pair(iterator(*this, std::move(st)), true);
	}

	TEM typename Container<T, Alloc>::const_iterator Container<T, Alloc>::cbegin() const
	{
		return const_iterator(*this);
	}

	TEM typename Container<T, Alloc>::const_iterator Container<T, Alloc>::cend() const
	{
		return const_iterator(*this, true);
	}

	TEM typename Container<T, Alloc>::const_iterator Container<T, Alloc>::cbegin()
	{
		return const_iterator(*this);
	}

	TEM typename Container<T, Alloc>::const_iterator Container<T, Alloc>::cbegin()
	{
		return const_iterator(*this, true);
	}

	TEM typename Container<T, Alloc>::iterator Container<T, Alloc>::begin() const
	{
		return iterator(*this);
	}

	TEM typename Container<T, Alloc>::iterator Container<T, Alloc>::end() const
	{
		return iterator(*this, true);
	}

	TEM typename Container<T, Alloc>::iterator Container<T, Alloc>::begin()
	{
		return iterator(*this);
	}

	TEM typename Container<T, Alloc>::iterator Container<T, Alloc>::begin()
	{
		return iterator(*this, true);
	}

	TEM void Container<T, Alloc>::Clear()
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::CreateNode()
	{
	}

	TEM TEM_U typename Container<T, Alloc>::Node* Container<T, Alloc>::CreateNode(U&& value)
	{
	}

	TEM typename Container<T, Alloc>::ControlBlock* Container<T, Alloc>::CreateControlBlock()
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetGrandParent(Node* node)
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetUncle(Node* node)
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetBrother(Node* node)
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetLeftBottomNode(Node* node)
	{
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetRightBottomNode(Node* node)
	{
	}

	TEM void Container<T, Alloc>::RotateLeft(Node* node)
	{
	}

	TEM void Container<T, Alloc>::RotateRight(Node* node)
	{
	}
}