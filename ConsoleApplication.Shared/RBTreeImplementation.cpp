#include "pch.h"
#include "RBTree.cpp"

namespace Containers
{
	template class RBTree<int32_t>;
	template __declspec(dllexport) std::pair<typename RBTree<int32_t>::iterator, bool> RBTree<int32_t>::Insert(int32_t&);
	template __declspec(dllexport) std::pair<typename RBTree<int32_t>::iterator, bool> RBTree<int32_t>::Insert(int32_t&&);
	template __declspec(dllexport) std::pair<typename RBTree<int32_t>::iterator, bool> RBTree<int32_t>::Erase(int32_t&);
	template __declspec(dllexport) std::pair<typename RBTree<int32_t>::iterator, bool> RBTree<int32_t>::Erase(int32_t&&);
}