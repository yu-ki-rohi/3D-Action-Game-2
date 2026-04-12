#include "RandomNode.h"
#include "DxLib.h"

using namespace BehaviorTree;

Status RandomNode::Tick(float elapsed_time_)
{
	if (runningIndex >= 0)
	{
		auto result = children[runningIndex]->Tick(elapsed_time_);

		if (result != Status::Running)
		{
			runningIndex = -1;
		}
		return result;
	}

	int judge = GetRand((int)children.size() - 1);

	auto result = children[judge]->Tick(elapsed_time_);

	if (result == Status::Running)
	{
		runningIndex = judge;
	}

	return result;
}