#include "InputBehavior.h"
#include <DxLib.h>
#include "../Systems/MemberFunctionPointerContainerBase.h"

InputBehavior::InputBehavior()
{

}

InputBehavior::~InputBehavior()
{
	Release();
}

void InputBehavior::Register(InputManager::Map map_, InputManager::State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	switch (state_)
	{
	case InputManager::State::Press:
		onPressBehaviors[map_].push_back(behavior_);
		break;
	case InputManager::State::Release:
		onReleaseBehaviors[map_].push_back(behavior_);
		break;
	case InputManager::State::Hold:
		onHoldBehaviors[map_].push_back(behavior_);
		break;
	}
}

void InputBehavior::Release()
{
	onPressBehaviors.clear();
	onReleaseBehaviors.clear();
	onHoldBehaviors.clear();
}

void InputBehavior::OnPress(InputManager::Map map_)
{
	Behave(map_, onPressBehaviors);
}

void InputBehavior::OnRelease(InputManager::Map map_)
{
	Behave(map_, onReleaseBehaviors);
}

void InputBehavior::OnHold(InputManager::Map map_)
{
	Behave(map_, onHoldBehaviors);
}

void InputBehavior::Behave(InputManager::Map map_, Behaviors& behaviors_)
{
	for (auto behavior : behaviors_[map_])
	{
		behavior->CallFunction();
	}
}