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
		Node* node = AllocTr::allocate(Alloc_, 1);

		try
		{
			AllocTr::construct(Alloc_, node);
		}
		catch (std::exception& ex)
		{
			AllocTr::deallocate(Alloc_, node, 1);
			throw;
		}

		return node;
	}

	TEM TEM_U typename Container<T, Alloc>::Node* Container<T, Alloc>::CreateNode(U&& value)
	{
		Node* node = AllocTr::allocate(Alloc_, 1);

		try
		{
			AllocTr::construct(Alloc_, node, std::forward<U>(value));
		}
		catch (std::exception& ex)
		{
			AllocTr::deallocate(Alloc_, node, 1);
			throw;
		}

		return node;
	}

	TEM typename Container<T, Alloc>::ControlBlock* Container<T, Alloc>::CreateControlBlock()
	{
		ControlBlock* controlBlock = ControlBlockAllocTr::allocate(ControlBlockAlloc_, 1);

		try
		{
			ControlBlockAllocTr::construct(ControlBlockAlloc_, controlBlock);
		}
		catch (std::exception& ex)
		{
			ControlBlockAllocTr::deallocate(ControlBlockAlloc_, controlBlock, 1);
			throw;
		}

		return controlBlock;
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetGrandParent(Node* node)
	{
		return node->Parent_ != FakeRoot_
			? node->Parent_->Parent_ != FakeRoot_
				? node->Parent_->Parent_
				: nullptr
			: nullptr;
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetUncle(Node* node)
	{
		Node* gp = GetGrandParent(node);

		return gp
			? gp->Left_ == node->Parent_
				? gp->Right_
				: gp->Left_
			: nullptr;
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetBrother(Node* node)
	{
		return node->Parent_ != FakeRoot_
			? node->Parent_->Left_ == node
				? node->Parent_->Right_
				: node->Parent_->Left_
			: nullptr;
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetLeftBottomNode(Node* node)
	{
		if (!node->Left_) return nullptr;

		node = node->Left_;

		while (node->Right_)
			node = node->Right_;

		return node;
	}

	TEM typename Container<T, Alloc>::Node* Container<T, Alloc>::GetRightBottomNode(Node* node)
	{
		if (!node->Right_) return nullptr;

		node = node->Right_;

		while (node->Left_)
			node = node->Left_;

		return node;
	}

	TEM void Container<T, Alloc>::RotateLeft(Node* node)
	{
		Node* pivot = node->Right_;
		pivot->Parent_ = node->Parent_;

		if (node->Parent_->Left_ == node)
			node->Parent_->Left_ = pivot;
		else
			node->Parent_->Right_ = pivot;

		node->Right_ = pivot->Left_;
		if (node->Right_) node->Right_->Parent_ = node;
		pivot->Left_ = node;
		node->Parent_ = pivot;
	}

	TEM void Container<T, Alloc>::RotateRight(Node* node)
	{
		Node* pivot = node->Left_;
		pivot->Parent_ = node->Parent_;

		if (node->Parent_->Left_ == node)
			node->Parent_->Left_ = pivot;
		else
			node->Parent_->Right_ = pivot;

		node->Left_ = pivot->Right_;
		if (node->Left_) node->Left_->Parent_ = node;
		pivot->Right_ = node;
		node->Parent_ = pivot;
	}

	TEM void Container<T, Alloc>::InsertCase1(Node* node)
	{
		if (node->Parent_ == FakeRoot_)
			node->IsMid_ = true;
		else
			InsertCase2(node);
	}

	TEM void Container<T, Alloc>::InsertCase2(Node* node)
	{
		if (node->Parent_->IsMid_)
			return;
		else
			InsertCase3(node);
	}

	TEM void Container<T, Alloc>::InsertCase3(Node* node)
	{
		Node* uncle;

		while ((uncle = GetUncle(node)) && !uncle->IsMid_)
		{
			node->Parent_->IsMid_ = true;
			uncle->IsMid_ = true;
			Node* gp = GetGrandParent(node);
			gp->IsMid_ = false;
			node = gp;

			if (node->Parent_ == FakeRoot_)
			{
				node->IsMid_ = true;
				return;
			}
			if (node->Parent_->IsMid_)
				return;
		}

		InsertCase4(node);
	}

	TEM void Container<T, Alloc>::InsertCase4(Node* node)
	{
		Node* gp = GetGrandParent(node);
	}

	TEM void Container<T, Alloc>::InsertCase5(Node* node)
	{
	}

	TEM bool Container<T, Alloc>::IsLeaf(Node* node)
	{
		return !node;
	}

	TEM void Container<T, Alloc>::DeleteWithTwoChild(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteWithOneChild(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteWithoutChild(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase1(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase2(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase3(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase4(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase5(Node* node)
	{
	}

	TEM void Container<T, Alloc>::DeleteCase6(Node* node)
	{
	}

	TEM void Container<T, Alloc>::ReplaceNode(Node* first, Node* second)
	{
	}

	TEM void Container<T, Alloc>::CompleteReplaceNode(Node* first, Node* second)
	{
	}

	TEM TEM_IS_CONST Container<T, Alloc>::common_iterator<IsConst>::common_iterator(Container<T, Alloc>& cont, std::stack<Node*>&& st)
		noexcept(std::is_nothrow_move_constructible_v<std::stack<Node*>>)
	{
	}
}