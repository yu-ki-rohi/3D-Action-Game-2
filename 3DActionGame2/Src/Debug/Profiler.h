#pragma once
#include "../Common.h"

#ifdef DEBUG

#include <DxLib.h>

#define STAMP_NUM 6

class Profiler
{
public:
	// �ō����Ԃ̋敪
	// �v�f����STAMP_NUM�ƍ��킹��
	enum class Type
	{
		FixedUpdate,
		Input,
		Update,
		Render,
		VSync,
		Other
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
	double stampedTime[STAMP_NUM]{};
	double showStampedTime[STAMP_NUM]{};


};

#endif