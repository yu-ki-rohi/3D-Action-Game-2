#pragma once
#include "EnemyAICommons.h"

struct Vector3;

// EnemyDirectiveを読み取るだけに限定するインターフェース
namespace EnemyAI
{
	class IEnemyDirectiveReader
	{
	public:
		virtual Vector3 GetDirectedPosition(unsigned char id_) const = 0;
		virtual Actions GetDirectedAciton(unsigned char id_) const = 0;

	};
}
