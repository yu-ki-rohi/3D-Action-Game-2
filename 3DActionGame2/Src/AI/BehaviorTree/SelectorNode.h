#pragma once
#include "CompositeNode.h"

namespace BehaviorTree
{
	class SelectorNode : public CompositeNode
	{
	public:
		// 一番最初に実行可能な子ノードを実行
		Status Tick(float elapsed_time_) override;
	};
}
