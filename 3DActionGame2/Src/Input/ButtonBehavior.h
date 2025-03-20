#pragma once
#include "InputBehavior.h"

class ButtonBehavior : public InputBehavior
{
public:
	ButtonBehavior();
	~ButtonBehavior();
public:
	void CheckInput(InputManager::Map map_, unsigned char current_button_state_);

private:
	unsigned char pastButtonState = 0;
};