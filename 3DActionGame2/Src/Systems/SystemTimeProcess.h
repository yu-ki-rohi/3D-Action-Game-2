#pragma once
#include "DxLib.h"
#include <memory>

/// <summary>
/// �o�ߎ��Ԃ𑪂邽�߂̏�����ێ�����N���X
/// *�����܂ŏ�����ێ����邾���Ŕ�r���鎞���͑����玝���Ă���K�v����
/// </summary>
class SystemTimeProcess
{
public:
	static float GetElapsedTime(LONGLONG& past_system_time_);
};