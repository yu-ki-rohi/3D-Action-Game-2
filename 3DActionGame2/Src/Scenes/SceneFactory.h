#pragma once
#include <memory>
#include "SceneBase.h"

class SceneFactory
{
public:
	static std::shared_ptr<SceneBase> Create(SceneBase::Type type_);
};