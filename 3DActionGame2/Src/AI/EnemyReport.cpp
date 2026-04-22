#include "EnemyReport.h"

using namespace EnemyAI;
EnemyReport::EnemyReport(int id_) :
	id(id_)
{

}

int EnemyReport::GetId()
{
	return id;
}

bool EnemyReport::IsAlive()
{
	return isAlive;
}

bool EnemyReport::HasReadDirective()
{
	return hasReadDirective;
}

bool EnemyReport::HasAchieved()
{
	return hasAchieved;
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