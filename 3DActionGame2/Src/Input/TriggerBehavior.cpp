#include "TriggerBehavior.h"
TriggerBehavior::TriggerBehavior()
{

}

TriggerBehavior::~TriggerBehavior()
{
	Release();
}
	
void TriggerBehavior::CheckInput(InputManager::Map map_, unsigned char depth_)
{
	const unsigned char border = 127;
	if (depth_ > border && pastTriggerState <= border)
	{
		OnPress(map_);
	}
	else if (depth_ <= border && pastTriggerState > border)
	{
		OnRelease(map_);
	}
	else if (depth_ > border && pastTriggerState > border)
	{
		OnHold(map_);
	}

	pastTriggerState = depth_;
}