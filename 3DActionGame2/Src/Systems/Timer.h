#pragma once
#include "MemberFunctionPointerContainer.h"
#include "TimerBase.h"

class App;

/// <summary>
/// 呼び出し書式 Timer timer(time, &t, &T::Func)
/// </summary>
template <class T, class V>
class Timer : public TimerBase
{
public:
	// メンバ関数ポインタ型の定義
	using MemberFunctionPointer = void (T::*)();
	Timer(const float time_, std::shared_ptr<V> existence_, T* obj_, MemberFunctionPointer func_) :
		TimerBase(time_),
		memberFunctionPointerContainer(MemberFunctionPointerContainer<T,V>(existence_, obj_, func_))
	{

	}

	void Update(float elapsedTime_) override
	{
		// ストップ中や生存フラグが切られているときは更新しない
		if (IsStop || !isActive) return;
		elapsedTime += elapsedTime_ * localTimeScale;
		if (elapsedTime > time)
		{
			// 関数を呼び出し、生存フラグを切る
			memberFunctionPointerContainer.CallFunction();
			isActive = false;
		}
	}

private:
	MemberFunctionPointerContainer<T,V> memberFunctionPointerContainer;
};
