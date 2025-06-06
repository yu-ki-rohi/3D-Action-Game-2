#pragma once
#include "MemberFunctionPointerContainer.h"
#include "TimerBase.h"

class App;

/// <summary>
/// �Ăяo������ Timer timer(time, &t, &T::Func)
/// </summary>
template <class T>
class Timer : public TimerBase
{
public:
	// �����o�֐��|�C���^�^�̒�`
	using MemberFunctionPointer = void (T::*)();
	Timer(const float time_, T* obj_, MemberFunctionPointer func_) :
		TimerBase(time_),
		memberFunctionPointerContainer(MemberFunctionPointerContainer<T>(obj_,func_))
	{

	}

	void Update(float elapsedTime_) override
	{
		// �X�g�b�v���␶���t���O���؂��Ă���Ƃ��͍X�V���Ȃ�
		if (IsStop || !isActive) return;
		elapsedTime += elapsedTime_;
		if (elapsedTime > TIME)
		{
			memberFunctionPointerContainer.CallFunction();
			isActive = false;
		}
	}

private:
	MemberFunctionPointerContainer<T> memberFunctionPointerContainer;
};
