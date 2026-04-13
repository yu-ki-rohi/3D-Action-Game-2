#pragma once
#include "EnemyAICommons.h"

struct Vector3;

// EnemyDirectiveを読み取るだけに限定するインターフェース
namespace EnemyAI
{
	class IEnemyDirectiveReader
	{
	public:
		virtual Vector3 GetDirectedPosition(int id_) const = 0;
		virtual Actions GetDirectedAciton(int id_) const = 0;

	};
}
