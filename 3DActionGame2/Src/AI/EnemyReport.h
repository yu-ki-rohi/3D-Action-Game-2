#pragma once
#include "IEnemyReportReader.h"
#include <memory>

struct Vector3;
class Transform;

// 各エネミーからの報告書のようなもの
namespace EnemyAI
{
	class EnemyReport : public IEnemyReportReader
	{
	public:
		EnemyReport(int id_);
	public:
		int GetId() const override;
		std::shared_ptr<const Transform> GetTransform() const override;
		bool IsAlive() const override;

		// 更新された指令書の内容をエネミーが確認したかどうかを返す(不要かも)
		bool HasReadDirective() const override;
		// エネミーが指令内容を達成したかどうかを返す
		bool HasAchieved() const override;

		void SetTransform(std::shared_ptr<const Transform> transform_);
		
		void ReadDirective();
		void AchieveMission();
		void Die();

		void OnDirectiveUpdated();

	private:
		const int id;
		bool hasReadDirective = false;
		bool hasAchieved = false;
		bool isAlive = true;

		std::shared_ptr<const Transform> transform;
	};
}