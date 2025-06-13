#include "KeyConfig.h"

unsigned char KeyConfig::GetConfig(Tag tag_)
{
	return configs[tag_];
}