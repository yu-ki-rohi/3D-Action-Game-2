#pragma once
#include "DxLib.h"

/// <summary>
/// �o�ߎ��Ԃ𑪂邽�߂̏�����ێ�����N���X
/// *�����܂ŏ�����ێ����邾���Ŕ�r���鎞���͑����玝���Ă���K�v����
/// 
/// �E�E�E����Time�N���X�ɂ������Ă������̂ł�?
/// </summary>
class SystemTimeProcess
{
public:
	static float GetElapsedTime(LONGLONG& past_system_time_);
};