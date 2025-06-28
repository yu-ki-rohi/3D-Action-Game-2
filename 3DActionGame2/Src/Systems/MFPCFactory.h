#pragma once
#include "MemberFunctionPointerContainer.h"

// MFPC : Member Function Pointer Container
class MFPCFactory
{
public:
	template <class T, class V>
	static std::shared_ptr<MemberFunctionPointerContainerBase> CreateMFPC(std::shared_ptr<V> existence_, T* obj_, void (T::* func_)())
	{
		return std::make_shared<MemberFunctionPointerContainer<T, V>>(existence_, obj_, func_);
	}
};