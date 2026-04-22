#include "EnemyCommander.h"

#include "../AI/EnemyCommanderBrain.h"
#include "../Systems/TimerFactory.h"

using namespace EnemyAI;

EnemyCommander::EnemyCommander(std::shared_ptr<EnemyDirective> directives_) :
	brain(std::make_shared<EnemyCommanderBrain>(directives_))
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