#pragma once
#include "CompositeNode.h"
namespace BehaviorTree
{
	class RandomNode : public CompositeNode
	{
	public:
		// ランダムな子ノードを実行
		Status Tick() override;
	};
}