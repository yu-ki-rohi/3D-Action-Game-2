#pragma once

namespace EnemyAI
{
	// 方位
	enum class Direction
	{
		N,
		NNE,
		NE,
		ENE,
		E,
		ESE,
		SE,
		SSE,
		S,
		SSW,
		SW,
		WSW,
		W,
		WNW,
		NW,
		NNW,
		End
	};

	// プレイヤーとの距離に関するenum
	enum class CombatRange
	{
		IN_FIGHT,
		CLOSE_RANGE,
		MIDDLE_RANGE,
		OUT_RANGE,
		End
	};

	// エネミーの行動に関するenum
	enum class Actions
	{
		STAND_BY,
		ATTACK,
		RUSH,
		GUARD,
		End
	};

}