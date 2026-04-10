#pragma once
#include "Node.h"
#include <functional>
namespace BehaviorTree
{
	class ConditionNode : public Node
	{
	public:
		// note:【explicit】(明示的な)  コピー初期化、暗黙の型変換を禁止するキーワード
		// todo: 挙動の違いの調査
		explicit ConditionNode(std::function<bool()> fn_);

		Status Tick() override;

	private:
		// note: 
		std::function<bool()> condition;
	};
}