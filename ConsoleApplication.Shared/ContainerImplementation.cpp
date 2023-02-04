#include "pch.h"
#include "Container.cpp"

namespace Containers
{
	template class Container<int32_t>;
	template __declspec(dllexport) std::pair<typename Container<int32_t>::iterator, bool> Container<int32_t>::Insert(int32_t&);
	template __declspec(dllexport) std::pair<typename Container<int32_t>::iterator, bool> Container<int32_t>::Insert(int32_t&&);
	template __declspec(dllexport) std::pair<typename Container<int32_t>::iterator, bool> Container<int32_t>::Erase(int32_t&);
	template __declspec(dllexport) std::pair<typename Container<int32_t>::iterator, bool> Container<int32_t>::Erase(int32_t&&);
}