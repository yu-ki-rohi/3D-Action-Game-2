#include "EnemyCommander.h"
#include "EnemyDirective.h"

#include "../Systems/TimerFactory.h"
#include "../Debug/DebugMessenger.h"
#include "DxLib.h"

using namespace EnemyAI;

EnemyCommander::EnemyCommander(std::shared_ptr<EnemyDirective> directives_) :
	directives(directives_)
{

}


void EnemyCommander::Command()
{
	// ひとまずの動作確認
	const int enemy_num = directives->GetDirectivesNum();

	for (int i = 0; i < enemy_num; ++i)
	{
		directives->SetDirective(i, (Direction)GetRand((int)Direction::End - 1), (CombatRange)GetRand((int)CombatRange::End - 1), (Actions)GetRand((int)Actions::ATTACK));
	}

	DebugMessenger::Log("コマンダー：配置変更");
	commandTimer = TimerFactory::CreateTimer(10.0f, shared_from_this(), this, &EnemyAI::EnemyCommander::Command);
}