#pragma once

namespace EnemyAI
{
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
		ATTACK0,
		ATTACK1,
		ATTACK2,
		End
	};

	struct DirectiveInfo
	{
		int Direction;
		CombatRange Range;
		Actions Action;

		DirectiveInfo(int dir_ = 0, CombatRange range_ = CombatRange::IN_FIGHT, Actions action_ = Actions::STAND_BY) :
			Direction(dir_),
			Range(range_),
			Action(action_){}
	};

}