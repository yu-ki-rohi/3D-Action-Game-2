#include "DataBase.h"
#include "AnimationFileList.h"
#include "ModelFileList.h"
#include "ShaderFileList.h"

std::shared_ptr<AnimationFileList> DataBase::GetAnimationFileList()
{
	return std::make_shared<AnimationFileList>();
}

std::shared_ptr<ModelFileList> DataBase::GetModelFileList()
{
	return std::make_shared<ModelFileList>();
}

std::shared_ptr<ShaderFileList> DataBase::GetShaderFileList()
{
	return std::make_shared<ShaderFileList>();
}