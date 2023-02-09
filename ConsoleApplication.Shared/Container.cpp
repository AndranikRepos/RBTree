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

		AllocTr::destroy(Alloc_, FakeRoot_);
		AllocTr::deallocate(Alloc_, FakeRoot_, 1);

		ControlBlockAllocTr::destroy(ControlBlockAlloc_, FakeRoot_);
		ControlBlockAllocTr::deallocate(ControlBlockAlloc_, FakeRoot_, 1);
	}

	TEM TEM_U std::pair<typename Container<T, Alloc>::iterator, bool> Container<T, Alloc>::Insert(U&& value)
	{
		std::stack<Node*> st;
		st.push(FakeRoot_);
		Node* node = CreateNode(std::forward<U>(value));

		if (!FakeRoot_->Left_)
		{
			FakeRoot_->Left_ = node;
			node->Parent_ = FakeRoot_;
		}
		else
		{
			Node* current = FakeRoot_->Left_;

			while (true)
			{
				if (current->Value_ > node->Value_)
				{
					if (current->Left_)
					{
						current = current->Left_;
						st.push(current);
					}
					else
					{
						current->Left_ = node;
						node->Parent_ = current;
						break;
					}
				}
				else
				{
					if (current->Right_)
					{
						current = current->Right_;
						st.push(current);
					}
					else
					{
						current->Right_ = node;
						node->Parent_ = current;
						break;
					}
				}
			}
		}

		InsertCase1(node);
		++Count_;

		return std::make_pair(iterator(*this, std::move(st)), true);
	}

	TEM TEM_U std::pair<typename Container<T, Alloc>::iterator, bool> Container<T, Alloc>::Erase(U&& value)
	{
		std::stack<Node*> st;
		st.push(FakeRoot_);
		Node* current = FakeRoot_->Left_;

		while (true)
		{
			if (current->Value_ == value)
			{
				DeleteWithTwoChild(current);
				return std::make_pair(iterator(*this, std::move(st)), true);
			}

			if (current->Value_ > value && current->Left_)
			{
				current = current->Left_;
				st.push(current);
			}
			else if (current->Value_ > value && current->Right_)
			{
				current = current->Right_;
				st.push(current);
			}
			else
			{
				break;
			}
		}

		return std::make_pair(iterator(*this, true), false);
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

	TEM typename Container<T, Alloc>::const_iterator Container<T, Alloc>::cend()
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

	TEM typename Container<T, Alloc>::iterator Container<T, Alloc>::end()
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
			ControlBlockAllocTr::construct(ControlBlockAlloc_, controlBlock, *this);
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

		if (gp)
		{
			if (node->Parent_->Right_ == node && gp->Left_ == node->Parent_)
			{
				RotateLeft(node->Parent_);
				node = node->Left_;
			}
			else if (node->Parent_->Left_ == node && gp->Right_ == node->Parent_)
			{
				RotateLeft(node->Parent_);
				node = node->Right_;
			}
		}

		InsertCase5(node);
	}

	TEM void Container<T, Alloc>::InsertCase5(Node* node)
	{
		Node* gp = GetGrandParent(node);

		if (gp)
		{
			node->Parent_->IsMid_ = true;
			gp->IsMid_ = false;

			if (gp->Left_ == node->Parent_)
				RotateRight(gp);
			else
				RotateLeft(gp);
		}
	}

	TEM bool Container<T, Alloc>::IsLeaf(Node* node)
	{
		return !node;
	}

	TEM void Container<T, Alloc>::DeleteWithTwoChild(Node* node)
	{
		if (!IsLeaf(node->Left_) && !IsLeaf(node->Right_))
		{
			CompleteReplaceNode(node, GetLeftBottomNode(node));
		}
		else
		{
			DeleteWithOneChild(node);
		}

		AllocTr::destroy(Alloc_, node);
		AllocTr::deallocate(Alloc_, node, 1);
	}

	TEM void Container<T, Alloc>::DeleteWithOneChild(Node* node)
	{
		if (IsLeaf(node->Left_) ^ IsLeaf(node->Right_))
		{
			Node* child = IsLeaf(node->Left_) ? node->Right_ : node->Left_;
			ReplaceNode(node, child);

			if (node->IsMid_)
			{
				if (!child->IsMid_)
					child->IsMid_ = true;
				else
					DeleteCase1(node);
			}
		}
		else
		{
			DeleteWithoutChild(node);
		}
	}

	TEM void Container<T, Alloc>::DeleteWithoutChild(Node* node)
	{
		if (node->Parent_->Left_ == node)
			node->Parent_->Left_ = nullptr;
		else
			node->Parent_->Right_ = nullptr;
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
		second->Parent_ = first->Parent_;

		if (first->Parent_->Left_ == first)
			first->Parent_->Left_ = second;
		else
			first->Parent_->Right_ = second;
	}

	TEM void Container<T, Alloc>::CompleteReplaceNode(Node* first, Node* second)
	{
		if (second->Parent_->Left_ == second)
			second->Parent_->Left_ = second->Left_;
		else
			second->Parent_->Right_ = second->Left_;

		if (second->Left_) second->Left_->Parent_ = second->Parent_;

		ReplaceNode(first, second);

		second->Left_ = first->Left_;
		if (second->Left_) second->Left_->Parent_ = second;

		second->Right_ = first->Right_;
		if (second->Right_) second->Right_->Parent_ = second;

		second->IsMid_ = first->IsMid_;
	}

	TEM Container<T, Alloc>::Node::Node() : Parent_{}, Value_{}, Left_{}, Right_{}, IsMid_{}
	{
	}

	TEM TEM_U Container<T, Alloc>::Node::Node(U&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
		: Parent_{}, Value_{ std::forward<U>(value) }, Left_{}, Right_{}, IsMid_{}
	{
	}

	TEM Container<T, Alloc>::ControlBlock::ControlBlock(Container<T, Alloc>& cont) noexcept : Cont_{ cont }
	{
	}

	TEM Container<T, Alloc>::ControlBlock::ControlBlock(Container<T, Alloc>& cont, std::stack<Node*>&& st)
		noexcept(std::is_nothrow_move_constructible_v<std::stack<Node*>>)
		: Cont_{ cont }, Stack_{ std::move(st) }
	{
	}

	TEM TEM_IS_CONST Container<T, Alloc>::common_iterator<IsConst>::common_iterator(Container<T, Alloc>& cont, std::stack<Node*>&& st)
		noexcept(std::is_nothrow_move_constructible_v<std::stack<Node*>>)
	{
		ControlBlock_ = cont.CreateControlBlock();
		ControlBlock_->Stack_ = std::move(st);
	}

	TEM TEM_IS_CONST TEM_CONT Container<T, Alloc>::common_iterator<IsConst>::common_iterator(Cont& cont)
	{
		ControlBlock_ = const_cast<std::decay_t<Cont>&>(cont).CreateControlBlock();
		ControlBlock_->Stack_.push(FakeRoot_);
		Node* node = cont.FakeRoot_;

		while (node)
		{
			ControlBlock_->Stack_.push(node);
			node = node->Left_;
		}
	}

	TEM TEM_IS_CONST TEM_CONT Container<T, Alloc>::common_iterator<IsConst>::common_iterator(Cont& cont, bool)
	{
		ControlBlock_ = cont.EndControlBlock_;
	}

	TEM TEM_IS_CONST Container<T, Alloc>::common_iterator<IsConst>::~common_iterator()
	{
		if (ControlBlock_ != ControlBlock_->Cont_.EndControlBlock_)
		{
			ControlBlockAllocTr::destroy(ControlBlock_->Cont_.ControlBlockAlloc_, ControlBlock_);
			ControlBlockAllocTr::deallocate(ControlBlock_->Cont_.ControlBlockAlloc_, ControlBlock_, 1);
		}
	}

	TEM TEM_IS_CONST Container<T, Alloc>::common_iterator<IsConst>::common_iterator(const common_iterator<IsConst>& iter)
	{
		if (iter.ControlBlock_ == iter.ControlBlock_->Cont_.EndControlBlock_)
		{
			ControlBlock_ = iter.ControlBlock_;
		}
		else
		{
			ControlBlock_ = iter.ControlBlock_->Cont_.CreateControlBlock();
			ControlBlock_->Stack_ = iter.ControlBlock_->Stack_;
		}
	}

	TEM TEM_IS_CONST Container<T, Alloc>::common_iterator<IsConst>::common_iterator(common_iterator<IsConst>&& iter) noexcept
		: ControlBlock_{ iter.ControlBlock_ }
	{
		iter.ControlBlock_ = nullptr;
	}

	TEM TEM_IS_CONST typename Container<T, Alloc>::template common_iterator<IsConst>&
		Container<T, Alloc>::common_iterator<IsConst>::operator=(const common_iterator<IsConst>& iter)
	{
		if (this == &iter || ControlBlock_ == iter.ControlBlock_)
			return *this;

		if (iter.ControlBlock_ == iter.ControlBlock_->Cont_.EndControlBlock_)
		{
			ControlBlock_ = iter.ControlBlock_;
		}
		else
		{
			ControlBlock_ = iter.ControlBlock_->Cont_.CreateControlBlock();
			ControlBlock_->Stack_ = iter.ControlBlock_->Stack_;
		}

		return *this;
	}

	TEM TEM_IS_CONST typename Container<T, Alloc>::template common_iterator<IsConst>&
		Container<T, Alloc>::common_iterator<IsConst>::operator=(common_iterator<IsConst>&& iter) noexcept
	{
		if (this == &iter)
			return *this;

		ControlBlock_ = iter.ControlBlock_;
		iter.ControlBlock_ = nullptr;

		return *this;
	}

	TEM TEM_IS_CONST std::conditional_t<IsConst, typename Container<T, Alloc>::const_reference,
		typename Container<T, Alloc>::reference>
		Container<T, Alloc>::common_iterator<IsConst>::operator*()
	{
		return ControlBlock_->Stack_.top().Value_;
	}

	TEM TEM_IS_CONST std::conditional_t<IsConst, const typename Container<T, Alloc>::template common_iterator<IsConst>&,
		typename Container<T, Alloc>::template common_iterator<IsConst>&>
		Container<T, Alloc>::common_iterator<IsConst>::operator++()
	{
		MoveNext();
		return *this;
	}

	TEM TEM_IS_CONST typename Container<T, Alloc>::template common_iterator<IsConst>
		Container<T, Alloc>::common_iterator<IsConst>::operator++(int)
	{
		auto iter = *this;
		operator++();
		return iter;
	}

	TEM TEM_IS_CONST std::conditional_t<IsConst, const typename Container<T, Alloc>::template common_iterator<IsConst>&,
		typename Container<T, Alloc>::template common_iterator<IsConst>&>
		Container<T, Alloc>::common_iterator<IsConst>::operator--()
	{
		MovePrev();
		return *this;
	}

	TEM TEM_IS_CONST typename Container<T, Alloc>::template common_iterator<IsConst>
		Container<T, Alloc>::common_iterator<IsConst>::operator--(int)
	{
		auto iter = *this;
		operator++();
		return iter;
	}

	TEM TEM_IS_CONST bool Container<T, Alloc>::common_iterator<IsConst>::operator!=(const common_iterator<IsConst>& iter)
	{
		return ControlBlock_->Stack_.top() != iter.ControlBlock_->Stack_.top();
	}

	TEM TEM_IS_CONST bool Container<T, Alloc>::common_iterator<IsConst>::operator==(const common_iterator<IsConst>& iter)
	{
		return !operator!=(iter);
	}

	TEM TEM_IS_CONST void Container<T, Alloc>::common_iterator<IsConst>::MoveNext()
	{
		Node* node = ControlBlock_->Stack_.top();
		ControlBlock_->Stack_.pop();
		node = node->Right_;

		while (node)
		{
			ControlBlock_->Stack_.push(node);
			node = node->Left_;
		}
	}

	TEM TEM_IS_CONST void Container<T, Alloc>::common_iterator<IsConst>::MovePrev()
	{
		Node* node = ControlBlock_->Stack_.top();
		ControlBlock_->Stack_.pop();
		node = node->Left_;

		while (node)
		{
			ControlBlock_->Stack_.push(node);
			node = node->Right_;
		}
	}

	TEM TEM_IS_CONST typename Container<T, Alloc>::Node* Container<T, Alloc>::common_iterator<IsConst>::operator&()
	{
		return ControlBlock_->Stack_.top();
	}
}