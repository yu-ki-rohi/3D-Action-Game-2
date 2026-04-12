#pragma once
#include "CompositeNode.h"

namespace BehaviorTree
{
	class SequenceNode : public CompositeNode
	{
	public:
		// 実行可能な子ノードを順番に実行
		Status Tick(float elapsed_time_) override;
	};
}