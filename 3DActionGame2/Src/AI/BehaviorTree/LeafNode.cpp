#include "LeafNode.h"

using namespace BehaviorTree;

LeafNode::LeafNode(std::function<Status(float)> function_) :
	function(std::move(function_))
{

}

Status LeafNode::Tick(float elapsed_time_)
{
	return function(elapsed_time_);
}