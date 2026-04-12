#pragma once
#include "Node.h"
#include <functional>

namespace BehaviorTree
{
	class LeafNode : public Node
	{
	public:
		// memo:【explicit】(明示的な)  コピー初期化、暗黙の型変換を禁止するキーワード
		// todo: 挙動の違いの調査
		explicit LeafNode(std::function<Status(float)> function_);
	public:
		Status Tick(float elapsed_time_) override;

	private:
		std::function<Status(float)> function;
	};
}