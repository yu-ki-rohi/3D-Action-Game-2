#include "SequenceNode.h"

using namespace BehaviorTree;

Status SequenceNode::Tick()
{
	for (int i = runningIndex; i < children.size(); ++i)
	{
		auto result = children[i]->Tick();
		if (result == Status::Success) { continue; }

		switch (result)
		{
		case Status::Running:
			runningIndex = i;
			break;
		case Status::Failure:
			runningIndex = 0;
			break;
		}

		return result;
	}
	runningIndex = 0;
	return Status::Success;
}