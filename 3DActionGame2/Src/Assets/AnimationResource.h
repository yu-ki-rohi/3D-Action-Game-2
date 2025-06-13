#pragma once
#include <unordered_map>
#include "../DataBase/DataKind.h"

struct AnimationResource
{
	std::unordered_map<AKind, int> Handles;
};