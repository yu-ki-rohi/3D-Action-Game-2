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
	// モデルファイル名読み込み
	shared_ptr<ModelFileList> model_file_list = DataBase::Instance().GetModelFileList();

	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓

	// モデルファイル名読み込み
	for (const auto& model_file : model_file_list->ModelFiles)
	{
		if (modelHandles[model_file.first] == nullptr)
		{
			modelHandles[model_file.first] = make_shared <ModelResource>();
		}
		modelHandles[model_file.first]->Handle = MV1LoadModel(model_file.second);
	}
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑

	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::LoadAnimation()
{
	using namespace std;
	// アニメーションファイル名読み込み
	shared_ptr<AnimationFileList> animation_file_list = DataBase::Instance().GetAnimationFileList();

	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓
	// アニメーションファイル読み込み
	for (const auto& animation_file : animation_file_list->AnimationFiles)
	{
		// MKindごとの各AKindにアニメーションを読み込んで保存
		if (animationHandles[animation_file.first] == nullptr)
		{
			animationHandles[animation_file.first] = make_shared <AnimationResource>();
		}
		animationHandles[animation_file.first]->Handles[animation_file.second.first] = MV1LoadModel(animation_file.second.second);
	}
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑

	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::LoadShader()
{
	using namespace std;
	// シェーダーフィル名読み込み
	shared_ptr<ShaderFileList> shader_file_list = DataBase::Instance().GetShaderFileList();

	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓
	// 頂点シェーダー読み込み
	for (const auto& vertex_shader_file : shader_file_list->VertexShaderFiles)
	{
		if (vertexShaderHandles[vertex_shader_file.first] == nullptr)
		{
			vertexShaderHandles[vertex_shader_file.first] = make_shared<VertexShaderResource>();
		}
		vertexShaderHandles[vertex_shader_file.first]->Handle = LoadVertexShader(vertex_shader_file.second);
	}

	// ピクセルシェーダー読み込み
	for (const auto& pixel_shader_file : shader_file_list->PixelShaderFiles)
	{
		if (pixelShaderHandles[pixel_shader_file.first] == nullptr)
		{
			pixelShaderHandles[pixel_shader_file.first] = make_shared<PixelShaderResource>();
		}
		pixelShaderHandles[pixel_shader_file.first]->Handle = LoadPixelShader(pixel_shader_file.second);
	}
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑
	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::MakeTmpScreen()
{
	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓
	tmpScreenHandle = MakeScreen(WindowSettings::WindowWidth, WindowSettings::WindowHeight, FALSE);
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑
	SetUseASyncLoadFlag(FALSE);
}

void AssetsManager::MakeShadowMap()
{
	// 作成する画像のフォーマットを不動小数点型で１チャンネル、１６ビットにする
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);

	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓
	shadowMapHandle = MakeScreen(2048, 2048, FALSE);
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑
	SetUseASyncLoadFlag(FALSE);

	// 設定を元に戻す
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
}