#pragma once
#include <unordered_map>

// トリガーをボタンとして扱うことがあるので
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
	// カメラの垂直方向反転
	bool isReverseVertical;
	// カメラの水平方向反転
	bool isReverseHorizontal;
	// キーの割り当て
	std::unordered_map<Tag, unsigned char> configs;
};