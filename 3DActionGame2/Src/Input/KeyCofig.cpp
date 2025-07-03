#include "KeyConfig.h"
#include <DxLib.h>

KeyConfig::KeyConfig() :
	isReverseHorizontal(false),
	isReverseVertical(false)
{
	configs[Tag::Decide] = XINPUT_BUTTON_B;
	configs[Tag::Cancel] = XINPUT_BUTTON_A;
	configs[Tag::Attack] = XINPUT_BUTTON_A;
	configs[Tag::Avoid] = XINPUT_BUTTON_B;
	configs[Tag::Dash] = XINPUT_BUTTON_RIGHT_SHOULDER;
}

unsigned char KeyConfig::GetConfig(Tag tag_)
{
	return configs[tag_];
}

bool KeyConfig::IsReverseVertical()
{
	return isReverseVertical;
}

bool KeyConfig::IsReverseHorizontal()
{
	return isReverseHorizontal;
}

void KeyConfig::SetConfig(Tag tag_, unsigned char button_)
{
	// DecideÇ©Cancelà»äOÇ≈îÌÇËÇ™î≠ê∂ÇµÇΩèÍçáÇÕì¸ÇÍë÷Ç¶
	if (tag_ != Tag::Decide && tag_ != Tag::Cancel) 
	{
		for (auto& config : configs)
		{
			if (config.second == button_ &&
				config.first != Tag::Decide &&
				config.first != Tag::Cancel)
			{
				config.second = configs[tag_];;
			}
		}
	}
	configs[tag_] = button_;
}

void KeyConfig::SwitchIsReverseVertical()
{
	isReverseVertical = !isReverseVertical;
}

void KeyConfig::SwitchIsReverseHorizontal()
{
	isReverseHorizontal = !isReverseHorizontal;
}
