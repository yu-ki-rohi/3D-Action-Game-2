#pragma once
#include <unordered_map>

// �g���K�[���{�^���Ƃ��Ĉ������Ƃ�����̂�
#define XINPUT_BUTTON_LEFT_TRIGGER 10
#define XINPUT_BUTTON_RIGHT_TRIGGER 11

class KeyConfig
{
public:
	enum class Tag
	{
		Decide,
		Cancel,
		Attack,
		Avoid,
		Dash
	};

public:
	KeyConfig();

public:
	unsigned char GetConfig(Tag tag_);

	bool IsReverseVertical();
	bool IsReverseHorizontal();

	void SetConfig(Tag tag_, unsigned char button_);

	void SwitchIsReverseVertical();
	void SwitchIsReverseHorizontal();


private:
	// �J�����̐����������]
	bool isReverseVertical;
	// �J�����̐����������]
	bool isReverseHorizontal;
	// �L�[�̊��蓖��
	std::unordered_map<Tag, unsigned char> configs;
};