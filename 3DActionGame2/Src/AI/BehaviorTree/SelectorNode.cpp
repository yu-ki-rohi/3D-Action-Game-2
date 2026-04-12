#include "SelectorNode.h"

using namespace BehaviorTree;

Status SelectorNode::Tick(float elapsed_time_)
{
	for (int i = runningIndex; i < children.size(); ++i)
	{
		auto result = children[i]->Tick(elapsed_time_);
		if (result == Status::Failure) { continue; }

		switch (result)
		{
		case Status::Running:
			runningIndex = i;
			break;
		case Status::Success:
			runningIndex = 0;
			break;
		}

		return result;
	}
	runningIndex = 0;
	return Status::Failure;
}