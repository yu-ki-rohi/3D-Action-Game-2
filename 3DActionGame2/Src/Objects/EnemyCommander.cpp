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
	localTimeScale = time_scale_;
	if (commandTimer == nullptr) { return; }
	commandTimer->SetLocalTimeScale(localTimeScale);
}

void EnemyCommander::MultiplyLocalTimeScaleBy(float multiplier_)
{
	localTimeScale *= multiplier_;
	if (commandTimer == nullptr) { return; }
	commandTimer->SetLocalTimeScale(localTimeScale);
}

void EnemyCommander::Start()
{
	brain->Start();
}

void EnemyCommander::Update(float elapsed_time)
{
	brain->Command();
}

int EnemyCommander::AllocateId()
{
	return brain->AllocateId();
}