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
}