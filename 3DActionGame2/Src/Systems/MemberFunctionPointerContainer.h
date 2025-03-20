#pragma once
#include "MemberFunctionPointerContainerBase.h"
/*
	�u�����o�֐��̊֐��|�C���^�𑼂̃N���X�ɓn���A����������s������@�v�ɂ��āA
	ChatGPT�ɃT���v���R�[�h�̒񎦂��˗����A
	�����Ɏ�����������̂ł��B
	���̃R�[�h�͈ȉ��ɂȂ�܂��B
	https://docs.google.com/document/d/1srepoKZf_39szieano4-QoJ59XDF1VViukK--uwZgPg/edit?tab=t.0#heading=h.c9v177hzx47j
*/

// �O���錾
// �T���v���e���v���[�g�����̂���
class App;

template <class T>
class MemberFunctionPointerContainer : public MemberFunctionPointerContainerBase
{
public:
	// �����o�֐��|�C���^�^�̒�`
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