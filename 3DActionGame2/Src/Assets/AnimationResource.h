#pragma once
#include <unordered_map>
#include <memory>
#include "../DataBase/DataKind.h"

struct AnimationResource
{
	std::unordered_map<AKind, int> Handles;

	std::unordered_map<AKind, std::shared_ptr<AnimationParameters>> AnimationParameters;
};
