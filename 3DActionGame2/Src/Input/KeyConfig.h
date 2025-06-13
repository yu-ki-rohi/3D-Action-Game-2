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
	// �J�����̐����������]
	bool isReverseVertical;
	// �J�����̐����������]
	bool isReverseHorizontal;
	// �L�[�̊��蓖��
	std::unordered_map<Tag, unsigned char> configs;
};