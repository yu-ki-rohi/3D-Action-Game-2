#include "DataBase.h"
#include "AnimationData.h"
#include "ModelData.h"
#include "EffectData.h"
#include "ShaderData.h"

std::shared_ptr<AnimationFileList> DataBase::GetAnimationFileList()
{
	return std::make_shared<AnimationFileList>();
}

std::shared_ptr<ModelFileList> DataBase::GetModelFileList()
{
	return std::make_shared<ModelFileList>();
}

std::shared_ptr<EffectFileList> DataBase::GetEffectFileList()
{
	return std::make_shared<EffectFileList>();
}

std::shared_ptr<ShaderFileList> DataBase::GetShaderFileList()
{
	return std::make_shared<ShaderFileList>();
}

std::shared_ptr<AnimationSpeedList> DataBase::GetAnimationSpeedList()
{
	return std::make_shared<AnimationSpeedList>();
}