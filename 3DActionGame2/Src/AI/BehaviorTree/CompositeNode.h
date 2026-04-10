#pragma once
#include "Node.h"
#include <vector>
#include <memory>

namespace BehaviorTree
{
	class CompositeNode : public Node
	{
	protected:

	protected:
		std::vector<std::unique_ptr<Node>> children;

		// Status::Running‚ÅI‚í‚Á‚½ƒm[ƒh‚ğT‚¦‚Ä‚¨‚­‚½‚ß‚Ì•Ï”
		int runningIndex = 0;
	};
}