#pragma once
#include "IEnemyReportReader.h"
// 各エネミーからの報告書のようなもの
namespace EnemyAI
{
	class EnemyReport : public IEnemyReportReader
	{
	public:
		EnemyReport(int id_);
	public:
		int GetId();
		bool IsAlive();
		bool HasReadDirective();
		bool HasAchieved();
		
		void ReadDirective();
		void AchieveMission();
		void Die();

		void OnDirectiveUpdated();

	private:
		const int id;
		bool hasReadDirective = false;
		bool hasAchieved = false;
		bool isAlive = true;
	};
}