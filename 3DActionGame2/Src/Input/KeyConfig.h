#pragma once
#include <unordered_map>

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
	unsigned char GetConfig(Tag tag_);

private:
	// カメラの垂直方向反転
	bool isReverseVertical;
	// カメラの水平方向反転
	bool isReverseHorizontal;
	// キーの割り当て
	std::unordered_map<Tag, unsigned char> configs;
};