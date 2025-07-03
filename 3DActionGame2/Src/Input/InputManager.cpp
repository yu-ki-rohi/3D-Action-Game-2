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

unsigned char InputManager::GetConfig(KeyConfig::Tag tag_)
{
	return config.GetConfig(tag_);
}

bool InputManager::IsReverseVertical()
{
	return config.IsReverseVertical();
}

bool InputManager::IsReverseHorizontal()
{
	return config.IsReverseHorizontal();
}

bool InputManager::IsSwitchedConfigDecideAndCancel()
{
	bool result = true;
	if (config.GetConfig(KeyConfig::Tag::Decide) == XINPUT_BUTTON_B)
	{
		result = false;
	}
	return result;
}

void InputManager::SetConfig(KeyConfig::Tag tag_, unsigned char button_)
{
	config.SetConfig(tag_, button_);
}

void InputManager::SwitchIsReverseVertical()
{
	config.SwitchIsReverseVertical();
}

void InputManager::SwitchIsReverseHorizontal()
{
	config.SwitchIsReverseHorizontal();
}


void InputManager::SwitchConfigDecideAndCancel()
{
	if (config.GetConfig(KeyConfig::Tag::Decide) == XINPUT_BUTTON_B)
	{
		config.SetConfig(KeyConfig::Tag::Decide, XINPUT_BUTTON_A);
		config.SetConfig(KeyConfig::Tag::Cancel, XINPUT_BUTTON_B);
	}
	else
	{
		config.SetConfig(KeyConfig::Tag::Decide, XINPUT_BUTTON_B);
		config.SetConfig(KeyConfig::Tag::Cancel, XINPUT_BUTTON_A);
	}
}

void InputManager::RegisterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	if (buttonType_ >= BUTTON_NUM) { return; }
	if (buttonType_ != XINPUT_BUTTON_LEFT_TRIGGER &&
		buttonType_ != XINPUT_BUTTON_RIGHT_TRIGGER)
	{
		buttons[buttonType_]->Register(map_, state_, behavior_);
	}
	else
	{
		triggers[buttonType_ - XINPUT_BUTTON_LEFT_TRIGGER]->Register(map_, state_, behavior_);
	}
}

void InputManager::RegisterBehave(Map map_, KeyConfig::Tag tag_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	RegisterBehave(map_, config.GetConfig(tag_), state_, behavior_);
}

void InputManager::RegisterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
{
	sticks[(int)stick_]->Register(map_, state_, behavior_);
}

void InputManager::RegisterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_)
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

void InputManager::AddObserver(Stick stick_, Map map_, std::weak_ptr<ObserverBase> observer_)
{
	sticks[(int)stick_]->AddObserver(map_, observer_);
}

void InputManager::AddObserver(Trigger trigger_, Map map_, std::weak_ptr<ObserverBase> observer_)
{
	triggers[(int)trigger_]->AddObserver(map_, observer_);
}

void InputManager::RemoveObserver(Stick stick_, Map map_, std::shared_ptr<ObserverBase> observer_)
{
	sticks[(int)stick_]->RemoveObserver(map_, observer_);
}

void InputManager::RemoveObserver(Trigger trigger_, Map map_, std::shared_ptr<ObserverBase> observer_)
{
	triggers[(int)trigger_]->RemoveObserver(map_, observer_);
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