#pragma once
#include "InputBehaviorWithSubject.h"

class TriggerBehavior : public InputBehaviorWithSubject
{
public:
	TriggerBehavior();
	~TriggerBehavior();
public:

public:
	void CheckInput(InputManager::Map map_, unsigned char depth_);

private:
	unsigned char pastTriggerState = 0;

};