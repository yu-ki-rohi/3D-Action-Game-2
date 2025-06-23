#pragma once

#include <memory>
#include "KeyConfig.h"

// *** •ÏX•s‰Â ***
#define BUTTON_NUM 16
#define STICK_NUM 2
#define TRIGGER_NUM 2
// ****************

class ButtonBehavior;
class StickBehavior;
class TriggerBehavior;
class MemberFunctionPointerContainerBase;
class ObserverBase;

/// <summary>
/// “ü—Í‚É‚æ‚éƒCƒxƒ“ƒgˆ—‚ğÀs‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX
/// </summary>

class InputManager
{
public:
	// unity‚ÌActionMap“I‚Èƒ‚ƒm
	// ‘€ì‚ğØ‚è‘Ö‚¦‚Ég—p(—á: InGame <=> Menu)
	enum class Map
	{
		Menu,
		Player,
		None		// Œ´‘¥None‚É‚Í‰½‚à“o˜^‚µ‚È‚¢
	};

	enum class State
	{
		Press,
		Release,
		Hold
	};

	enum class Stick
	{
		Left,
		Right
	};

	enum class Trigger
	{
		Left,
		Right
	};

public:
	// “ü—Í‚Ì‚Ó‚é‚Ü‚¢‚ğ“o˜^
	// ‘æˆêˆø”:“o˜^æ‚Ìƒ}ƒbƒvA‘æ“ñˆø”:“o˜^‚·‚éƒ{ƒ^ƒ“A‘æOˆø”:“o˜^‚·‚é“ü—Íó‘ÔA‘ælˆø”:“o˜^‚·‚é‚Ó‚é‚Ü‚¢
	void RegisterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ‘æˆêˆø”:“o˜^æ‚Ìƒ}ƒbƒvA‘æ“ñˆø”:“o˜^‚·‚éƒXƒeƒBƒbƒNA‘æOˆø”:“o˜^‚·‚é“ü—Íó‘ÔA‘ælˆø”:“o˜^‚·‚é‚Ó‚é‚Ü‚¢
	void RegisterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ‘æˆêˆø”:“o˜^æ‚Ìƒ}ƒbƒvA‘æ“ñˆø”:“o˜^‚·‚éƒgƒŠƒK[A‘æOˆø”:“o˜^‚·‚é“ü—Íó‘ÔA‘ælˆø”:“o˜^‚·‚é‚Ó‚é‚Ü‚¢
	void RegisterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ‘æˆêˆø”:“o˜^æ‚Ìƒ}ƒbƒvA‘æ“ñˆø”:“o˜^‚·‚é‚Ó‚é‚Ü‚¢‚Ì•ª—ŞA‘æOˆø”:“o˜^‚·‚é“ü—Íó‘ÔA‘ælˆø”:“o˜^‚·‚é‚Ó‚é‚Ü‚¢
	void RegisterBehave(Map map_, KeyConfig::Tag tag_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);

	// “o˜^‚³‚ê‚½‚Ó‚é‚Ü‚¢‚ğ‘S‰ğœ
	void Clear();

	// “ü—Í’l‚Ì‘—Mæ‚ğ’Ç‰Á
	void AddObserver(Stick stick_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void AddObserver(Trigger trigger_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(Stick stick_, Map map_, std::shared_ptr<ObserverBase> observer_);
	void RemoveObserver(Trigger trigger_, Map map_, std::shared_ptr<ObserverBase> observer_);

	// “ü—Íó‘Ô‚ÌŠm”F‹y‚Ñ“o˜^‚³‚ê‚½ˆ—‚ÌÀs
	void CheckInput();

	// ƒ}ƒbƒv‚ÌØ‘Ö
	void ChangeMap(Map map_);

private:
	Map currentMap = Map::Menu;
	std::shared_ptr<ButtonBehavior> buttons[BUTTON_NUM];
	std::shared_ptr<StickBehavior> sticks[STICK_NUM];
	std::shared_ptr<TriggerBehavior> triggers[TRIGGER_NUM];

	KeyConfig config;

#pragma region ƒVƒ“ƒOƒ‹ƒgƒ““o˜^
public:
	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

private:
	InputManager();
	~InputManager();
#pragma endregion

};