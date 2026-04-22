#include "EnemyCommanderBrain.h"
#include "EnemyDirective.h"
#include "EnemyReportHandler.h"
#include "IEnemyReportReader.h"

#include "../Systems/TimerFactory.h"
#include "../Debug/DebugMessenger.h"
#include "DxLib.h"

using namespace EnemyAI;

EnemyCommanderBrain::EnemyCommanderBrain(std::shared_ptr<EnemyDirective> directives_, std::shared_ptr<EnemyAI::EnemyReportHandler> report_handler_) :
	directives(directives_),
	reportHandler(report_handler_)
{

}

int EnemyCommanderBrain::AllocateId()
{
	int id = reportHandler->GetIdHasDead();

	if (id < 0)
	{
		return directives->AddDirective();
	}
	return id;
}

void EnemyCommanderBrain::Command()
{
	// ひとまずの動作確認
	const int enemy_num = directives->GetDirectivesNum();

	for (int i = 0; i < enemy_num; ++i)
	{
		directives->SetDirective(i, (Direction)GetRand((int)Direction::End - 1), (CombatRange)GetRand((int)CombatRange::End - 1), (Actions)GetRand((int)Actions::ATTACK));
	}
	DebugMessenger::Log("コマンダー：配置変更");
}