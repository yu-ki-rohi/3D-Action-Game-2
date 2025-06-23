#pragma once

#include <memory>
#include "KeyConfig.h"

// *** �ύX�s�� ***
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
/// ���͂ɂ��C�x���g���������s���邽�߂̃N���X
/// </summary>

class InputManager
{
public:
	// unity��ActionMap�I�ȃ��m
	// �����؂�ւ��Ɏg�p(��: InGame <=> Menu)
	enum class Map
	{
		Menu,
		Player,
		None		// ����None�ɂ͉����o�^���Ȃ�
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
	// ���͎��̂ӂ�܂���o�^
	// ������:�o�^��̃}�b�v�A������:�o�^����{�^���A��O����:�o�^������͏�ԁA��l����:�o�^����ӂ�܂�
	void RegisterBehave(Map map_, unsigned char buttonType_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ������:�o�^��̃}�b�v�A������:�o�^����X�e�B�b�N�A��O����:�o�^������͏�ԁA��l����:�o�^����ӂ�܂�
	void RegisterBehave(Map map_, Stick stick_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ������:�o�^��̃}�b�v�A������:�o�^����g���K�[�A��O����:�o�^������͏�ԁA��l����:�o�^����ӂ�܂�
	void RegisterBehave(Map map_, Trigger trigger_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);
	// ������:�o�^��̃}�b�v�A������:�o�^����ӂ�܂��̕��ށA��O����:�o�^������͏�ԁA��l����:�o�^����ӂ�܂�
	void RegisterBehave(Map map_, KeyConfig::Tag tag_, State state_, std::shared_ptr<MemberFunctionPointerContainerBase> behavior_);

	// �o�^���ꂽ�ӂ�܂���S����
	void Clear();

	// ���͒l�̑��M���ǉ�
	void AddObserver(Stick stick_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void AddObserver(Trigger trigger_, Map map_, std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(Stick stick_, Map map_, std::shared_ptr<ObserverBase> observer_);
	void RemoveObserver(Trigger trigger_, Map map_, std::shared_ptr<ObserverBase> observer_);

	// ���͏�Ԃ̊m�F�y�ѓo�^���ꂽ�����̎��s
	void CheckInput();

	// �}�b�v�̐ؑ�
	void ChangeMap(Map map_);

private:
	Map currentMap = Map::Menu;
	std::shared_ptr<ButtonBehavior> buttons[BUTTON_NUM];
	std::shared_ptr<StickBehavior> sticks[STICK_NUM];
	std::shared_ptr<TriggerBehavior> triggers[TRIGGER_NUM];

	KeyConfig config;

#pragma region �V���O���g���o�^
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