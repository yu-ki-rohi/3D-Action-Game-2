#include "StickBehavior.h"


StickBehavior::StickBehavior()
{

}
StickBehavior::~StickBehavior()
{
	Release();
}
	
void StickBehavior::CheckInput(InputManager::Map map_, short x_, short y_)
{
	float x = x_ / (float)SHRT_MAX;
	float y = y_ / (float)SHRT_MAX;

	float currentSqrDepth = x * x + y * y;

	float border = 0.3f;

	for (auto& subject : subjects)
	{
		subject.second.NotifyObservers(0.0f, 0.0f);
	}

	if (currentSqrDepth > border)
	{
		subjects[map_].NotifyObservers(x, y);
	}

	if (currentSqrDepth > border && pastSqrDepth <= border)
	{
		OnPress(map_);
	}
	else if (currentSqrDepth <= border && pastSqrDepth > border)
	{
		OnRelease(map_);
	}
	else if (currentSqrDepth > border && pastSqrDepth > border)
	{
		OnHold(map_);
	}

	pastSqrDepth = currentSqrDepth;
}