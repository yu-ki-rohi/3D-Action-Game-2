#pragma once
#include "Node.h"
#include <memory>
#include "../../Systems/TimerFactory.h"
#include "../../DataBase/DataKind.h"

namespace BehaviorTree
{
	class TestAttack : public Node
	{
	public:
		Status Tick() override;

	private:
		AKind animationKind;
		std::shared_ptr<TimerBase> enableColliderTimer;
		std::shared_ptr<TimerBase> disableColliderTimer;
	};
}