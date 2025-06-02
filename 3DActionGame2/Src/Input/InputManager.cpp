#include "InputManager.h"
#include "DxLib.h"
#include "ButtonBehavior.h"
#include "TriggerBehavior.h"
#include "StickBehavior.h"


InputManager::InputManager()
{
	for (auto& button : buttons)
	{
		button = std::make_shared<ButtonBehavior>();
	}
	for (auto& stick : sticks)
	{
		stick = std::make_shared<StickBehavior>();
	}
	for (auto& trigger : triggers)
	{
		trigger = std::make_shared<TriggerBehavior>();
	}
}

InputManager::~InputManager()
{

}

void InputManager::RegisiterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	if (buttonType_ >= BUTTON_NUM) return;
	buttons[buttonType_]->Register(map_, state_, behavior_);
}

void InputManager::RegisiterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	sticks[(int)stick_]->Register(map_, state_, behavior_);
}

void InputManager::RegisiterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	triggers[(int)trigger_]->Register(map_, state_, behavior_);
}

void InputManager::Clear()
{
	for (auto& button : buttons)
	{
		button->Release();
	}
	for (auto& stick : sticks)
	{
		stick->Release();
		stick->ClearObservers();
	}
	for (auto& trigger : triggers)
	{
		trigger->Release();
		trigger->ClearObservers();
	}
}

void InputManager::AddObserver(Stick stick_, std::weak_ptr<ObserverBase> observer_)
{
	sticks[(int)stick_]->AddObserver(observer_);
}

void InputManager::AddObserver(Trigger trigger_, std::weak_ptr<ObserverBase> observer_)
{
	triggers[(int)trigger_]->AddObserver(observer_);
}

void InputManager::RemoveObserver(Stick stick_, std::shared_ptr<ObserverBase> observer_)
{
	sticks[(int)stick_]->RemoveObserver(observer_);
}

void InputManager::RemoveObserver(Trigger trigger_, std::shared_ptr<ObserverBase> observer_)
{
	triggers[(int)trigger_]->RemoveObserver(observer_);
}

void InputManager::CheckInput()
{
	XINPUT_STATE xinput_state;
	if (GetJoypadXInputState(DX_INPUT_PAD1, &xinput_state) == -1) return;
	for (int i = 0; i < BUTTON_NUM; i++)
	{
		buttons[i]->CheckInput(currentMap, xinput_state.Buttons[i]);
	}

	// Lスティック
	sticks[0]->CheckInput(currentMap, xinput_state.ThumbLX, xinput_state.ThumbLY);

	// Rスティック
	sticks[1]->CheckInput(currentMap, xinput_state.ThumbRX, xinput_state.ThumbRY);


	// Lトリガー
	triggers[0]->CheckInput(currentMap, xinput_state.LeftTrigger);

	// Rトリガー
	triggers[1]->CheckInput(currentMap, xinput_state.RightTrigger);

}

void InputManager::ChangeMap(Map map_)
{
	currentMap = map_;
}