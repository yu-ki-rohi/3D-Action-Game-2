#pragma once
#include "MemberFunctionPointerContainerBase.h"
/*
	「メンバ関数の関数ポインタを他のクラスに渡し、そこから実行する方法」について、
	ChatGPTにサンプルコードの提示を依頼し、
	そこに手を加えたものです。
	元のコードは以下になります。
	https://docs.google.com/document/d/1srepoKZf_39szieano4-QoJ59XDF1VViukK--uwZgPg/edit?tab=t.0#heading=h.c9v177hzx47j
*/

// 前方宣言
// サンプルテンプレート引数のため
class App;

template <class T>
class MemberFunctionPointerContainer : public MemberFunctionPointerContainerBase
{
public:
	// メンバ関数ポインタ型の定義
	using MemberFunctionPointer = void (T::*)();

	MemberFunctionPointerContainer(T* obj_, MemberFunctionPointer func_) :
		targetObj(obj_),
		targetFunc(func_)
	{

	}

	void CallFunction() override
	{
		if (!targetObj || !targetFunc) return;
		(targetObj->*targetFunc)();
	}

private:
	T* targetObj = nullptr;
	MemberFunctionPointer targetFunc = nullptr;
};