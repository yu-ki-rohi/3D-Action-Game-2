#include "EnemyReport.h"
#include "../Debug/DebugMessenger.h"

using namespace EnemyAI;
EnemyReport::EnemyReport(int id_) :
	id(id_)
{

}

int EnemyReport::GetId() const
{
	return id;
}

bool EnemyReport::IsAlive() const
{
	return isAlive;
}

bool EnemyReport::HasReadDirective() const
{
	return hasReadDirective;
}

bool EnemyReport::HasAchieved() const
{
	return hasAchieved;
}

std::shared_ptr<const Transform> EnemyReport::GetTransform() const
{
	return transform;
}

void EnemyReport::SetTransform(std::shared_ptr<const Transform> transform_)
{
	transform = transform_;
	if (transform == nullptr)
	{
		DebugMessenger::LogWarning("transform‚ªnullptr‚Å‚·");
	}
	else
	{
		DebugMessenger::Log("transform‚ðƒZƒbƒg‚µ‚Ü‚µ‚½");
	}
}

void EnemyReport::ReadDirective()
{
	hasReadDirective = true;
}

void EnemyReport::AchieveMission()
{
	hasAchieved = true;
}

void EnemyReport::Die()
{
	isAlive = false;
}

void EnemyReport::OnDirectiveUpdated()
{
	hasReadDirective = false;
	hasAchieved = false;
}