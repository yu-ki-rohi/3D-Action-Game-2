#pragma once
#include "MemberFunctionPointerContainer.h"

// MFPC : Member Function Pointer Container


class MFPCFactory
{
public:
	// T : メンバ関数を保有するクラス
	// V : TまたはTの基底クラス
	
	// 第一引数 インスタンスを指すスマートポインタ
	// 第二引数 第一引数と同一のインスタンスを指す生ポインタ
	// 第三引数 実行させたいメンバ関数へのポインタ
	template <class T, class V>
	static std::shared_ptr<MemberFunctionPointerContainerBase> CreateMFPC(std::shared_ptr<V> existence_, T* obj_, void (T::* func_)())
	{
		return std::make_shared<MemberFunctionPointerContainer<T, V>>(existence_, obj_, func_);
	}
};