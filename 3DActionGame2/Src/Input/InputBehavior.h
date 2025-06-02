#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "InputManager.h"

class MemberFunctionPointerContainerBase;

/// <summary>
/// 入力時のふるまいに関する基底クラス
/// On/Offのみ
/// </summary>
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
	// ボタンが押されたときに発火する関数を保管
	// Map -> Behavior
	Behaviors onPressBehaviors;
	Behaviors onReleaseBehaviors;
	Behaviors onHoldBehaviors;
};