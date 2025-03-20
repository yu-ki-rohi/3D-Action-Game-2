#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "InputManager.h"

class MemberFunctionPointerContainerBase;

class InputBehavior
{
public:
	InputBehavior();
	~InputBehavior();
public:
	
public:
	void Register(InputManager::Map map_, InputManager::State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	void Release();

protected:
	void OnPress(InputManager::Map map_);
	void OnRelease(InputManager::Map map_);
	void OnHold(InputManager::Map map_);

	using Behaviors = std::unordered_map<InputManager::Map, std::vector<std::shared_ptr<MemberFunctionPointerContainerBase>>>;
	void Behave(InputManager::Map map_, Behaviors& behaviors_);

private:
	// ƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚Æ‚«‚É”­‰Î‚·‚éŠÖ”‚ð•ÛŠÇ
	// Map -> Behavior
	Behaviors onPressBehaviors;
	Behaviors onReleaseBehaviors;
	Behaviors onHoldBehaviors;
};