#include "ButtonBehavior.h"
#include <DxLib.h>
#include "../Systems/MemberFunctionPointerContainerBase.h"

ButtonBehavior::ButtonBehavior()
{

}

ButtonBehavior::~ButtonBehavior()
{
	Release();
}
	

void ButtonBehavior::CheckInput(InputManager::Map map_, unsigned char current_button_state_)
{
	if (pastButtonState == 0 && current_button_state_ == 1)
	{
		OnPress(map_);
	}
	else if (pastButtonState == 1 && current_button_state_ == 0)
	{
		OnRelease(map_);
	}
	else if (pastButtonState == 1 && current_button_state_ == 1)
	{
		OnHold(map_);
	}
	pastButtonState = current_button_state_;
}
