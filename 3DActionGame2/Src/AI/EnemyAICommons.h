#pragma once

namespace EnemyAI
{
	// 方位とバイナリの対応
	enum class Direction
	{
		N = 0x00,
		NNE = 0x01,
		NE = 0x02,
		ENE = 0x03,
		E = 0x04,
		ESE = 0x05,
		SE = 0x06,
		SSE = 0x07,
		S = 0x08,
		SSW = 0x09,
		SW = 0x0a,
		WSW = 0x0b,
		W = 0x0c,
		WNW = 0x0d,
		NW = 0x0e,
		NNW = 0x0f,
	};

	// プレイヤーとの距離に関するenum
	enum class CombatRange
	{
		IN_FIGHT,
		CLOSE_RANGE,
		MIDDLE_RANGE,
		OUT_RANGE
	};

	// エネミーの行動に関するenum
	enum class Actions
	{
		STAND_BY,
		ATTACK,
		RUSH,
		GUARD
	};

}