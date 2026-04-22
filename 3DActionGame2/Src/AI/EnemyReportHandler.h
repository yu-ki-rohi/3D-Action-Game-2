#pragma once
#include<memory>
#include<vector>

// •ñ‘‚ğæ‚è‚Ü‚Æ‚ß‚éƒNƒ‰ƒX
namespace EnemyAI
{
	class EnemyReport;
	class IEnemyReportReader;

	class EnemyReportHandler
	{
	public:
		bool HasAchievedEverybody();
		std::shared_ptr<IEnemyReportReader> GetReport(int id_);
		int GetIdHasDead();

		void OnDirectiveUpdated();

		void AddReport(std::shared_ptr<EnemyReport> report_);

	private:
		std::vector<std::shared_ptr<EnemyReport>> reports;
	};
}