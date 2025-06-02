#pragma once
#include <DxLib.h>
#include "../Common.h"

#define STAMP_NUM 4

class Profiler
{
public:
	// ‘ÅŠÔ‚Ìí—Ş
	// —v‘f”‚ÍSTAMP_NUM‚Æ‡‚í‚¹‚é
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

	// •`‰æ—p‚Ì•Ï”‚ÉŠi”[‚·‚éˆ—
	void Update();


	void ResetTimes();

private:
	LONGLONG previousSystemTime;

	// “Y‚¦š‚ÍType‚Åw’è
	float stampedTime[STAMP_NUM]{};
	float showStampedTime[STAMP_NUM]{};


};