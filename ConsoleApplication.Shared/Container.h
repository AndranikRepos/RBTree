#pragma once
#include <cstdint>
#include <memory>
#include <cstring>
#include <utility>
#include <type_traits>

#ifdef SHARED_LIB
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#define __TEM_T template < typename T
#define TEM_T __TEM_T >
#define __TEM_T_ALLOC __TEM_T , template < typename > typename Alloc
#define TEM_DEF __TEM_T_ALLOC = std::allocator >
#define TEM __TEM_T_ALLOC >
#define SIZE_TYPE std::size_t
#define INT_TYPE = int32_t
#define __TEM_CONT template < typename Cont , typename
#define TEM_CONT __TEM_CONT >
#define TEM_CONT_DEF __TEM_CONT = std::enable_if_t <	\
	!std::is_same_v < std::decay_t < Cont > , common_iterator < IsConst > >	\
	&& !std::is_base_of_v < common_iterator < IsConst > , std::decay_t < Cont > >	\
	>	\
>
#define TEM_U template < typename U >
#define TEM_IS_CONST template < bool IsConst >

namespace Containers
{
	TEM_DEF class DLLEXPORT Container
	{
	public:
		using size_type = SIZE_TYPE;
		using int_type = INT_TYPE;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

	private:
		struct Node
		{
			Node* Parent_;
			value_type Value_;
			Node* Left_;
			Node* Right_;
			bool IsMid_;
			Node();
			TEM_U Node(U&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>);
		};

		struct ControlBlock
		{
			Container<T, Alloc>& Cont_;
			std::stack<Node*> Stack_;
			ControlBlock(Container<T, Alloc>& cont) noexcept;
			ControlBlock(Container<T, Alloc>& cont, std::stack<Node*>&& st) noexcept(std::is_nothrow_move_constructible_v<std::stack<Node*>>);
		};

		TEM_IS_CONST class DLLEXPORT common_iterator
		{
			friend class Container<T, Alloc>;
			ControlBlock* ControlBlock_;
			common_iterator(Container<T, Alloc>& cont, std::stack<Node*>&& st)
				noexcept(std::is_nothrow_move_constructible_v<std::stack<Node*>>);
		};

		Alloc<Node> Alloc_;
		using AllocTr = std::allocator_traits<decltype(Alloc_)>;

		Alloc<ControlBlock> Alloc_;
		using ControlBlockAllocTr = std::allocator_traits<decltype(ControlBlockAlloc_)>;

		Node* FakeRoot_;
		ControlBlock* EndControlBlock_;
		size_type Count_;

		Node* CreateNode();
		TEM_U Node* CreateNode(U&& value);
		ControlBlock* CreateControlBlock();
		Node* GetGrandParent(Node* node);
		Node* GetUncle(Node* node);
		Node* GetBrother(Node* node);
		Node* GetLeftBottomNode(Node* node);
		Node* GetRightBottomNode(Node* node);
		void RotateLeft(Node* node);
		void RotateRight(Node* node);
		void InsertCase1(Node* node);
		void InsertCase2(Node* node);
		void InsertCase3(Node* node);
		void InsertCase4(Node* node);
		void InsertCase5(Node* node);
		void (Node* node);

	public:
		using const_iterator = common_iterator<true>;
		using iterator = common_iterator<false>;

		Container();
		virtual ~Container();
		TEM_U std::pair<iterator, bool> Insert(U&& value);
		TEM_U std::pair<iterator, bool> Erase(U&& value);
		const_iterator cbegin() const;
		const_iterator cend() const;
		const_iterator cbegin();
		const_iterator cend();
		iterator begin() const;
		iterator end() const;
		iterator begin();
		iterator end();
		void Clear();
	};
}