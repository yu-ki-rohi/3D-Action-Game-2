#pragma once
#include <memory>

class Transform;

// EnemyReportを読み取り専用で受け取るインターフェース
namespace EnemyAI
{
	class IEnemyReportReader
	{
	public:
		virtual int GetId() const = 0;
		virtual std::shared_ptr<const Transform> GetTransform() const = 0;
		virtual bool IsAlive() const = 0;
		virtual bool HasReadDirective() const = 0;
		virtual bool HasAchieved() const = 0;
	};
}