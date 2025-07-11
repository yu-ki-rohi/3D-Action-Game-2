#pragma once
#include "../Common.h"

#ifdef DEBUG

#include <DxLib.h>

#define STAMP_NUM 6

class Profiler
{
public:
	// 打刻時間の区分
	// 要素数はSTAMP_NUMと合わせる
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

	// 描画用の変数に格納する処理
	void Update();


	void ResetTimes();

private:
	LONGLONG previousSystemTime;

	// 添え字はTypeで指定
	float stampedTime[STAMP_NUM]{};
	float showStampedTime[STAMP_NUM]{};


};

#endif