#include "RandomNode.h"
#include "DxLib.h"

using namespace BehaviorTree;

Status RandomNode::Tick()
{
	if (runningIndex >= 0)
	{
		auto result = children[runningIndex]->Tick();

		if (result != Status::Running)
		{
			runningIndex = -1;
		}
		return result;
	}

	int judge = GetRand((int)children.size() - 1);

	auto result = children[judge]->Tick();

	if (result == Status::Running)
	{
		runningIndex = judge;
	}

	return result;
}