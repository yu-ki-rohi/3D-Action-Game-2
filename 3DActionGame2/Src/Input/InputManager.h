#pragma once

#include <memory>
#include "KeyConfig.h"

// *** 変更不可 ***
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
/// 入力によるイベント処理を実行するためのクラス
/// </summary>

class InputManager
{
public:
	// unityのActionMap的なモノ
	// 操作を切り替えに使用(例: InGame <=> Menu)
	enum class Map
	{
		Menu,
		Player,
		None		// 原則Noneには何も登録しない
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
	unsigned char GetConfig(KeyConfig::Tag tag_);

	bool IsReverseVertical();
	bool IsReverseHorizontal();
	bool IsSwitchedConfigDecideAndCancel();

	void SetConfig(KeyConfig::Tag tag_, unsigned char button_);

	void SwitchIsReverseVertical();
	void SwitchIsReverseHorizontal();
	void SwitchConfigDecideAndCancel();

	// 入力時のふるまいを登録
	// 第一引数:登録先のマップ、第二引数:登録するボタン、第三引数:登録する入力状態、第四引数:登録するふるまい
	void RegisterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// 第一引数:登録先のマップ、第二引数:登録するスティック、第三引数:登録する入力状態、第四引数:登録するふるまい
	void RegisterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// 第一引数:登録先のマップ、第二引数:登録するトリガー、第三引数:登録する入力状態、第四引数:登録するふるまい
	void RegisterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// 第一引数:登録先のマップ、第二引数:登録するふるまいの分類、第三引数:登録する入力状態、第四引数:登録するふるまい
	void RegisterBehave(Map map_, KeyConfig::Tag tag_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);

	// 登録されたふるまいを全解除
	void Clear();

	// 入力値の送信先を追加
	void AddObserver(Stick stick_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void AddObserver(Trigger trigger_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(Stick stick_, Map map_, std::shared_ptr<ObserverBase> observer_);
	void RemoveObserver(Trigger trigger_, Map map_, std::shared_ptr<ObserverBase> observer_);

	// 入力状態の確認及び登録された処理の実行
	void CheckInput();

	// マップの切替
	void ChangeMap(Map map_);


private:
	Map currentMap = Map::Menu;
	std::shared_ptr<ButtonBehavior> buttons[BUTTON_NUM];
	std::shared_ptr<StickBehavior> sticks[STICK_NUM];
	std::shared_ptr<TriggerBehavior> triggers[TRIGGER_NUM];

	KeyConfig config;

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