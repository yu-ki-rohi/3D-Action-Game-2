#pragma once
#include "InputBehaviorWithSubject.h"

class StickBehavior : public InputBehaviorWithSubject
{
public:
	StickBehavior();
	~StickBehavior();
public:

	void CheckInput(InputManager::Map map_, short x_, short y_);

private:
	float pastSqrDepth = 0.0f;
};