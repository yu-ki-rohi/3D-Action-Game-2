#include "AssetsManager.h"
#include "AnimationResource.h"
#include "ModelResource.h"
#include "ShaderResource.h"
#include "../DataBase/DataBase.h"
#include "../DataBase/AnimationFileList.h"
#include "../DataBase/ModelFileList.h"
#include "../DataBase/ShaderFileList.h"
#include "../Common.h"
#include <DxLib.h>

AssetsManager::AssetsManager() :
	tmpScreenHandle(-1),
	shadowMapHandle(-1)
{

}

AssetsManager::~AssetsManager()
{
	DeleteAll();
}

void AssetsManager::Load()
{
	LoadModel();

	LoadAnimation();

	LoadShader();

	tmpScreenHandle = MakeScreen(WindowSettings::WindowWidth, WindowSettings::WindowHeight, FALSE);

	MakeShadowMap();
}

void AssetsManager::DeleteAll()
{
	for (const auto& model : modelHandles)
	{
		MV1DeleteModel(model.second->Handle);
	}

	for (const auto& model : animationHandles)
	{
		for (const auto& animation : model.second->Handles)
		{
			MV1DeleteModel(animation.second);
		}
	}

	InitShader();

	DeleteGraph(tmpScreenHandle);
	DeleteGraph(shadowMapHandle);
}

const std::shared_ptr<ModelResource> AssetsManager::GetModel(MKind kind_)
{
	return modelHandles[kind_];
}

const std::shared_ptr<AnimationResource> AssetsManager::GetAnimation(MKind kind_)
{
	return animationHandles[kind_];
}

const std::shared_ptr<VertexShaderResource> AssetsManager::GetVertexShader(VSKind kind_)
{
	return vertexShaderHandles[kind_];
}

const std::shared_ptr<PixelShaderResource> AssetsManager::GetPixelShader(PSKind kind_)
{
	return pixelShaderHandles[kind_];
}

int AssetsManager::GetTmpScreenHandle()
{
	return tmpScreenHandle;
}

int AssetsManager::GetShadowMapHandle()
{
	return shadowMapHandle;
}

void AssetsManager::LoadModel()
{
	using namespace std;
	// ���f���t�@�C�����ǂݍ���
	shared_ptr<ModelFileList> model_file_list = DataBase::Instance().GetModelFileList();
	// ���f���t�@�C�����ǂݍ���
	for (const auto& model_file : model_file_list->ModelFiles)
	{
		modelHandles[model_file.first]->Handle = MV1LoadModel(model_file.second);
	}
}

void AssetsManager::LoadAnimation()
{
	using namespace std;
	// �A�j���[�V�����t�@�C�����ǂݍ���
	shared_ptr<AnimationFileList> animation_file_list = DataBase::Instance().GetAnimationFileList();
	// �A�j���[�V�����t�@�C���ǂݍ���
	for (const auto& animation_file : animation_file_list->AnimationFiles)
	{
		// MKind���Ƃ̊eAKind�ɃA�j���[�V������ǂݍ���ŕۑ�
		animationHandles[animation_file.first]->Handles[animation_file.second.first] = MV1LoadModel(animation_file.second.second);
	}
}

void AssetsManager::LoadShader()
{
	using namespace std;
	// �V�F�[�_�[�t�B�����ǂݍ���
	shared_ptr<ShaderFileList> shader_file_list = DataBase::Instance().GetShaderFileList();
	// ���_�V�F�[�_�[�ǂݍ���
	for (const auto& vertex_shader_file : shader_file_list->VertexShaderFiles)
	{
		vertexShaderHandles[vertex_shader_file.first]->Handle = MV1LoadModel(vertex_shader_file.second);
	}
	// �s�N�Z���V�F�[�_�[�ǂݍ���
	for (const auto& pixel_shader_file : shader_file_list->PixelShaderFiles)
	{
		pixelShaderHandles[pixel_shader_file.first]->Handle = MV1LoadModel(pixel_shader_file.second);
	}

}

void AssetsManager::MakeShadowMap()
{
	// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);
	shadowMapHandle = MakeScreen(2048, 2048, FALSE);

	// �ݒ�����ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
}