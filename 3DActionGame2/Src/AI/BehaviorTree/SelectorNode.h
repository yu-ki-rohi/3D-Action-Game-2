#pragma once
#include "CompositeNode.h"

namespace BehaviorTree
{
	class SelectorNode : CompositeNode
	{
	public:
		// 一番最初に実行可能な子ノードを実行
		Status Tick() override;
	};
}
