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

	MakeTmpScreen();

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

	SetUseASyncLoadFlag(TRUE);
	// �������񓯊��ǂݍ��݊֐��͂������牺�ŌĂԁ�����

	// ���f���t�@�C�����ǂݍ���
	for (const auto& model_file : model_file_list->ModelFiles)
	{
		if (modelHandles[model_file.first] == nullptr)
		{
			modelHandles[model_file.first] = make_shared <ModelResource>();
		}
		modelHandles[model_file.first]->Handle = MV1LoadModel(model_file.second);
	}
	// �������񓯊��ǂݍ��݊֐��͂��������ŌĂԁ�����

	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::LoadAnimation()
{
	using namespace std;
	// �A�j���[�V�����t�@�C�����ǂݍ���
	shared_ptr<AnimationFileList> animation_file_list = DataBase::Instance().GetAnimationFileList();

	SetUseASyncLoadFlag(TRUE);
	// �������񓯊��ǂݍ��݊֐��͂������牺�ŌĂԁ�����
	// �A�j���[�V�����t�@�C���ǂݍ���
	for (const auto& animation_file : animation_file_list->AnimationFiles)
	{
		// MKind���Ƃ̊eAKind�ɃA�j���[�V������ǂݍ���ŕۑ�
		if (animationHandles[animation_file.first] == nullptr)
		{
			animationHandles[animation_file.first] = make_shared <AnimationResource>();
		}
		animationHandles[animation_file.first]->Handles[animation_file.second.first] = MV1LoadModel(animation_file.second.second);
	}
	// �������񓯊��ǂݍ��݊֐��͂��������ŌĂԁ�����

	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::LoadShader()
{
	using namespace std;
	// �V�F�[�_�[�t�B�����ǂݍ���
	shared_ptr<ShaderFileList> shader_file_list = DataBase::Instance().GetShaderFileList();

	SetUseASyncLoadFlag(TRUE);
	// �������񓯊��ǂݍ��݊֐��͂������牺�ŌĂԁ�����
	// ���_�V�F�[�_�[�ǂݍ���
	for (const auto& vertex_shader_file : shader_file_list->VertexShaderFiles)
	{
		if (vertexShaderHandles[vertex_shader_file.first] == nullptr)
		{
			vertexShaderHandles[vertex_shader_file.first] = make_shared<VertexShaderResource>();
		}
		vertexShaderHandles[vertex_shader_file.first]->Handle = LoadVertexShader(vertex_shader_file.second);
	}

	// �s�N�Z���V�F�[�_�[�ǂݍ���
	for (const auto& pixel_shader_file : shader_file_list->PixelShaderFiles)
	{
		if (pixelShaderHandles[pixel_shader_file.first] == nullptr)
		{
			pixelShaderHandles[pixel_shader_file.first] = make_shared<PixelShaderResource>();
		}
		pixelShaderHandles[pixel_shader_file.first]->Handle = LoadPixelShader(pixel_shader_file.second);
	}
	// �������񓯊��ǂݍ��݊֐��͂��������ŌĂԁ�����
	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::MakeTmpScreen()
{
	SetUseASyncLoadFlag(TRUE);
	// �������񓯊��ǂݍ��݊֐��͂������牺�ŌĂԁ�����
	tmpScreenHandle = MakeScreen(WindowSettings::WindowWidth, WindowSettings::WindowHeight, FALSE);
	// �������񓯊��ǂݍ��݊֐��͂��������ŌĂԁ�����
	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::MakeShadowMap()
{
	// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);

	SetUseASyncLoadFlag(TRUE);
	// �������񓯊��ǂݍ��݊֐��͂������牺�ŌĂԁ�����
	shadowMapHandle = MakeScreen(2048, 2048, FALSE);
	// �������񓯊��ǂݍ��݊֐��͂��������ŌĂԁ�����
	SetUseASyncLoadFlag(FALSE);

	// �ݒ�����ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
}