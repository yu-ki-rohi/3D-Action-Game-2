#pragma once
#include <DxLib.h>
#include "../Common.h"

#define STAMP_NUM 4

class Profiler
{
public:
	// �ō����Ԃ̎��
	// �v�f����STAMP_NUM�ƍ��킹��
	enum class Type
	{
		FixedUpdate,
		Update,
		Render,
		VSync
	};


public:
	void Stamp(Type type_);
	void Render();

	// �`��p�̕ϐ��Ɋi�[���鏈��
	void Update();


	void ResetTimes();

private:
	LONGLONG previousSystemTime;

	// �Y������Type�Ŏw��
	float stampedTime[STAMP_NUM]{};
	float showStampedTime[STAMP_NUM]{};


};