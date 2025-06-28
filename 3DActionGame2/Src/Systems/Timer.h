#pragma once
#include "MemberFunctionPointerContainer.h"
#include "TimerBase.h"

class App;

/// <summary>
/// �Ăяo������ Timer timer(time, &t, &T::Func)
/// </summary>
template <class T, class V>
class Timer : public TimerBase
{
public:
	// �����o�֐��|�C���^�^�̒�`
	using MemberFunctionPointer = void (T::*)();
	Timer(const float time_, std::shared_ptr<V> existence_, T* obj_, MemberFunctionPointer func_) :
		TimerBase(time_),
		memberFunctionPointerContainer(MemberFunctionPointerContainer<T,V>(existence_, obj_, func_))
	{

	}

	void Update(float elapsedTime_) override
	{
		// �X�g�b�v���␶���t���O���؂��Ă���Ƃ��͍X�V���Ȃ�
		if (IsStop || !isActive) return;
		elapsedTime += elapsedTime_ * localTimeScale;
		if (elapsedTime > time)
		{
			// �֐����Ăяo���A�����t���O��؂�
			memberFunctionPointerContainer.CallFunction();
			isActive = false;
		}
	}

private:
	MemberFunctionPointerContainer<T,V> memberFunctionPointerContainer;
};
