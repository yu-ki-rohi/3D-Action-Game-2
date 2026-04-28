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

void EnemyCommanderBrain::Start()
{
	SetupSequences();
	InitialCommand();
}

void EnemyCommanderBrain::InitialCommand()
{
	directives->SetTransformOfRepresentativeEnemy(reportHandler->GetReport(0)->GetTransform());

	const int char_bit_num = 8;

	for (int i = 0; i < participatingEnemyNum; ++i)
	{
		directives->SetDirective(idCenterLeftRight[i], static_cast<unsigned char>(sequences[currentSequence][currentStep] >> (char_bit_num * i)));
	}
	reportHandler->OnDirectiveUpdated();
	DebugMessenger::Log("コマンダー：配置変更");
}

void EnemyCommanderBrain::Command()
{
	//
	if (reportHandler->HasAchievedEverybody() == false) { return; }

	++currentStep;
	if (currentStep >= sequences[currentSequence].size())
	{
		currentStep = 0;

		currentSequence = GetRand(sequences.size() - 1);
		SetNextCenter(GetRand(participatingEnemyNum - 1));
		directives->SetTransformOfRepresentativeEnemy(reportHandler->GetReport(idCenterLeftRight[Center])->GetTransform());
	}
	const int char_bit_num = 8;

	for (int i = 0; i < participatingEnemyNum; ++i)
	{
		directives->SetDirective(idCenterLeftRight[i], static_cast<unsigned char>(sequences[currentSequence][currentStep] >> (char_bit_num * i)));
	}

	reportHandler->OnDirectiveUpdated();
	DebugMessenger::Log("コマンダー：配置変更");
}

void EnemyCommanderBrain::SetupSequences()
{
	// todo: データの分離
	const int char_bit_num = 8;
	unsigned int directives;
	DirectiveInfo dir_info[participatingEnemyNum] = {};
	auto meke_directives = [&directives,&dir_info]()
	{
		directives = 0;
		for (int i = 0; i < participatingEnemyNum; ++i)
		{
			unsigned int direct = EnemyDirective::MakeDirective(dir_info[i]);
			directives |= direct << (char_bit_num * i);
		}
	};

	// directivesに格納する値は
	// 0x00/右/左/中
	{
		std::vector<unsigned int> sequence;

		dir_info[0] = DirectiveInfo(0, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[1] = DirectiveInfo(1, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-1, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[1] = DirectiveInfo(1, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[2] = DirectiveInfo(-1, CombatRange::IN_FIGHT, Actions::ATTACK);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[1] = DirectiveInfo(6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[1] = DirectiveInfo(6, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[2] = DirectiveInfo(-6, CombatRange::IN_FIGHT, Actions::ATTACK);
		meke_directives();
		sequence.push_back(directives);

		sequences.push_back(sequence);
	}
	
	{
		std::vector<unsigned int> sequence;

		dir_info[0] = DirectiveInfo(0, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[1] = DirectiveInfo(6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[1] = DirectiveInfo(6, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[2] = DirectiveInfo(-6, CombatRange::IN_FIGHT, Actions::ATTACK);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[1] = DirectiveInfo(6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		sequences.push_back(sequence);
	}

	{
		std::vector<unsigned int> sequence;

		dir_info[0] = DirectiveInfo(0, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[1] = DirectiveInfo(6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::MIDDLE_RANGE, Actions::STAND_BY);
		dir_info[1] = DirectiveInfo(1, CombatRange::CLOSE_RANGE, Actions::ATTACK);
		dir_info[2] = DirectiveInfo(-1, CombatRange::CLOSE_RANGE, Actions::ATTACK);
		meke_directives();
		sequence.push_back(directives);

		dir_info[0] = DirectiveInfo(0, CombatRange::IN_FIGHT, Actions::ATTACK);
		dir_info[1] = DirectiveInfo(6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		dir_info[2] = DirectiveInfo(-6, CombatRange::CLOSE_RANGE, Actions::STAND_BY);
		meke_directives();
		sequence.push_back(directives);

		sequences.push_back(sequence);
	}

	
}

void EnemyCommanderBrain::SetNextCenter(int id_)
{
	int index = 0;

	for (int i = 0; i < participatingEnemyNum; ++i)
	{
		if (idCenterLeftRight[i] == id_)
		{
			index = i;
			break;
		}
	}
	if (index == 0) { return; }

	switch (index)
	{
	case Left:
		idCenterLeftRight[Left] = idCenterLeftRight[Right];
		idCenterLeftRight[Right] = idCenterLeftRight[Center];
		idCenterLeftRight[Center] = id_;
		break;
	case Right:
		idCenterLeftRight[Right] = idCenterLeftRight[Left];
		idCenterLeftRight[Left] = idCenterLeftRight[Center];
		idCenterLeftRight[Center] = id_;
		break;
	}
}