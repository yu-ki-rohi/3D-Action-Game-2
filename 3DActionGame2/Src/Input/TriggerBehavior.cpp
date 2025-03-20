#include "TriggerBehavior.h"
TriggerBehavior::TriggerBehavior()
{

}

TriggerBehavior::~TriggerBehavior()
{
	Release();
}
	
void TriggerBehavior::CheckInput(InputManager::Map map_, unsigned char d_)
{
	const unsigned char border = 127;
	if (d_ > border && pastTriggerState <= d_)
	{
		OnPress(map_);
	}
	else if (d_ <= border && pastTriggerState > d_)
	{
		OnRelease(map_);
	}
	else if (d_ > border && pastTriggerState > d_)
	{
		OnHold(map_);
	}
}