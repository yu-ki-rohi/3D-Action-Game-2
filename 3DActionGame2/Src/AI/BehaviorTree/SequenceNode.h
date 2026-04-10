#pragma once
#include "CompositeNode.h"

namespace BehaviorTree
{
	class SequenceNode : CompositeNode
	{
	public:
		// 実行可能な子ノードを順番に実行
		Status Tick() override;
	};
}