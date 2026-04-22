#pragma once

// 派生したテンプレートクラスを一括で動かすため

class MemberFunctionPointerContainerBase
{
public:
	virtual ~MemberFunctionPointerContainerBase() = default;
public:
	virtual void CallFunction() = 0;
};