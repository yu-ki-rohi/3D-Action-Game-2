#pragma once
#include <DxLib.h>
#include "../Common.h"

#define STAMP_NUM 4

class Profiler
{
public:
	// 打刻時間の種類
	// 要素数はSTAMP_NUMと合わせる
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

	// 描画用の変数に格納する処理
	void Update();


	void ResetTimes();

private:
	LONGLONG previousSystemTime;

	// 添え字はTypeで指定
	float stampedTime[STAMP_NUM]{};
	float showStampedTime[STAMP_NUM]{};


};