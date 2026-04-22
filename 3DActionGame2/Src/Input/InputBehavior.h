#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "InputManager.h"

class MemberFunctionPointerContainerBase;

/// <summary>
/// “ü—Í‚Ì‚Ó‚é‚Ü‚¢‚ÉŠÖ‚·‚éŠî’êƒNƒ‰ƒX
/// On/Off‚Ì‚İ
/// </summary>
class InputBehavior
{
public:
	InputBehavior();
	virtual ~InputBehavior();
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
	// “ü—Í‚ÌuŠÔ‚É”­‰Î‚·‚éŠÖ”‚ğ•ÛŠÇ
	Behaviors onPressBehaviors;
	// “ü—ÍI—¹‚ÌuŠÔ‚É”­‰Î‚·‚éŠÖ”‚ğ•ÛŠÇ
	Behaviors onReleaseBehaviors;
	// “ü—Í’†‚É”­‰Î‚·‚éŠÖ”‚ğ•ÛŠÇ
	Behaviors onHoldBehaviors;
};