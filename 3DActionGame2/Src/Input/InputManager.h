#pragma once

#include <memory>

#define BUTTON_NUM 16
#define STICK_NUM 2
#define TRIGGER_NUM 2

class ButtonBehavior;
class StickBehavior;
class TriggerBehavior;
class MemberFunctionPointerContainerBase;
class ObserverBase;

/// <summary>
/// 入力によるイベント処理を実行するためのクラス
/// </summary>

class InputManager
{
public:
	enum class Map
	{
		Menu,
		Player,
		None
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

	void RegisiterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	void RegisiterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	void RegisiterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);

	void Clear();

	void AddObserver(Stick stick_, std::weak_ptr<ObserverBase> observer_);
	void AddObserver(Trigger trigger_, std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(Stick stick_, std::shared_ptr<ObserverBase> observer_);
	void RemoveObserver(Trigger trigger_, std::shared_ptr<ObserverBase> observer_);

	void CheckInput();

	void ChangeMap(Map map_);

private:
	Map currentMap = Map::Menu;
	std::shared_ptr<ButtonBehavior> buttons[BUTTON_NUM];
	std::shared_ptr<StickBehavior> sticks[STICK_NUM];
	std::shared_ptr<TriggerBehavior> triggers[TRIGGER_NUM];

#pragma region シングルトン登録
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