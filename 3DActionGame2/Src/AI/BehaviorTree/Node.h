#pragma once
// Nodeの実装にあたって参考にしたWebページ
// https://qiita.com/tsukino_/items/af765bde77898e26a31a

namespace BehaviorTree
{
	enum class Status
	{
		Success,
		Failure,
		Running
	};

	class Node
	{
	public:
		virtual ~Node() = default;
		virtual Status Tick() = 0;
	};
}
