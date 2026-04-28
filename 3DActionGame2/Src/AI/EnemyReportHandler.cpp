#include "EnemyReportHandler.h"
#include "EnemyReport.h"

#include "../Debug/DebugMessenger.h"

using namespace EnemyAI;

bool EnemyReportHandler::HasAchievedEverybody()
{
	for (auto& repo : reports)
	{
		if (repo->IsAlive() == false) { continue; }
		if (repo->HasAchieved() == false)
		{
			return false;
		}
	}
	DebugMessenger::Log("レポートハンドラー：全員がミッション達成");
	return true;
}

std::shared_ptr<IEnemyReportReader> EnemyReportHandler::GetReport(int id_)
{
	if (id_ < 0 || id_ >= reports.size())
	{
		DebugMessenger::LogError("範囲外が指定されました");
		return nullptr;
	}
	return reports[id_];
}

int EnemyReportHandler::GetIdHasDead()
{
	for (auto& repo : reports)
	{
		if (repo->IsAlive() == false)
		{
			return repo->GetId();
		}
	}
	return -1;
}

void EnemyReportHandler::OnDirectiveUpdated()
{
	for (auto& repo : reports)
	{
		repo->OnDirectiveUpdated();
	}
}

void EnemyReportHandler::AddReport(std::shared_ptr<EnemyReport> report_)
{
	// 最終的な想定
	// コマンダーからGetIdHasDead()を叩く→idが有効値ならその値を用いて、無効なら指令書の数をidとしてエネミー生成→
	// エネミーが自身のidでレポートを作成→そのレポートをこの関数で登録

	if (report_ == nullptr) { return; }
	int id = report_->GetId();

	if (id < 0 || id > reports.size())
	{
		DebugMessenger::LogError("レポートのIDが不正です");
		return;
	}
	else if (id == reports.size())
	{
		reports.push_back(report_);
	}
	else
	{
		reports[id] = report_;
	}
}