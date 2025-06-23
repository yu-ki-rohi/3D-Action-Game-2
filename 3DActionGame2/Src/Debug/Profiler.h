#pragma once
#include "../Common.h"

#ifdef DEBUG

#include <DxLib.h>

#define STAMP_NUM 6

class Profiler
{
public:
	// ‘ÅŠÔ‚Ì‹æ•ª
	// —v‘f”‚ÍSTAMP_NUM‚Æ‡‚í‚¹‚é
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

	// •`‰æ—p‚Ì•Ï”‚ÉŠi”[‚·‚éˆ—
	void Update();


	void ResetTimes();

private:
	LONGLONG previousSystemTime;

	// “Y‚¦š‚ÍType‚Åw’è
	double stampedTime[STAMP_NUM]{};
	double showStampedTime[STAMP_NUM]{};


};

#endif