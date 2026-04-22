#include "EnemyCommander.h"

#include "../AI/EnemyCommanderBrain.h"
#include "../Systems/TimerFactory.h"

using namespace EnemyAI;

EnemyCommander::EnemyCommander(std::shared_ptr<EnemyDirective> directives_, std::shared_ptr<EnemyAI::EnemyReportHandler> report_handler_) :
	brain(std::make_shared<EnemyCommanderBrain>(directives_, report_handler_))
{

}

ObjectBase::Tag EnemyCommander::GetTag() const
{
	return Tag::Enemy;
}

void EnemyCommander::SetLocalTimeScale(float time_scale_)
{

}

void EnemyCommander::MultiplyLocalTimeScaleBy(float multiplier_)
{

}

void EnemyCommander::Start()
{

}

void EnemyCommander::Update(float elapsed_time)
{
	if (commandTimer == nullptr || commandTimer->IsActive() == false)
	{
		commandTimer = TimerFactory::CreateTimer(10.0f, brain, &*brain, &EnemyAI::EnemyCommanderBrain::Command);
	}
}

int EnemyCommander::AllocateId()
{
	return brain->AllocateId();
}