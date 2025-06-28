#include "AssetsManager.h"
#include "AnimationResource.h"
#include "ModelResource.h"
#include "EffectResource.h"
#include "ShaderResource.h"
#include "../DataBase/DataBase.h"
#include "../DataBase/AnimationData.h"
#include "../DataBase/EffectData.h"
#include "../DataBase/ModelData.h"
#include "../DataBase/ShaderData.h"
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
	
	LoadEffect();

	LoadShader();

	MakeTmpScreen();

	MakeShadowMap();
}

void AssetsManager::DeleteAll()
{
	for (const auto& model : modelResource)
	{
		MV1DeleteModel(model.second->Handle);
	}

	for (const auto& model : animationResource)
	{
		for (const auto& animation : model.second->Handles)
		{
			MV1DeleteModel(animation.second);
		}
	}

	for (auto& effect : effectResource)
	{
		for (int i = 0; i < effect.second->ImageNum; i++)
		{
			DeleteGraph(effect.second->Handles[i]);
		}
		delete[](effect.second->Handles);
		effect.second->Handles = nullptr;
	}

	InitShader();

	DeleteGraph(tmpScreenHandle);
	DeleteGraph(shadowMapHandle);
}

const std::shared_ptr<ModelResource> AssetsManager::GetModel(MKind kind_)
{
	return modelResource[kind_];
}

const std::shared_ptr<AnimationResource> AssetsManager::GetAnimation(MKind kind_)
{
	return animationResource[kind_];
}

const std::shared_ptr<EffectResource> AssetsManager::GetEffect(EKind kind_)
{
	return effectResource[kind_];
}

const std::shared_ptr<VertexShaderResource> AssetsManager::GetVertexShader(VSKind kind_)
{
	return vertexShaderResource[kind_];
}

const std::shared_ptr<PixelShaderResource> AssetsManager::GetPixelShader(PSKind kind_)
{
	return pixelShaderResource[kind_];
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
		if (modelResource[model_file.first] == nullptr)
		{
			modelResource[model_file.first] = make_shared <ModelResource>();
		}
		modelResource[model_file.first]->Handle = MV1LoadModel(model_file.second);
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
		if (animationResource[animation_file.first] == nullptr)
		{
			animationResource[animation_file.first] = make_shared <AnimationResource>();
		}
		animationResource[animation_file.first]->Handles[animation_file.second.first] = MV1LoadModel(animation_file.second.second);
	}
	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑
	SetUseASyncLoadFlag(FALSE);

	//Animationの再生速度を設定
	shared_ptr<AnimationSpeedList> animation_speed_list = DataBase::Instance().GetAnimationSpeedList();
	for (const auto& animation_speed : animation_speed_list->AnimationSpeeds)
	{
		// MKindごとの各AKindにアニメーションを読み込んで保存
		if (animationResource[animation_speed.first] == nullptr)
		{
			animationResource[animation_speed.first] = make_shared <AnimationResource>();
		}
		animationResource[animation_speed.first]->AnimSpeed[animation_speed.second.first] = animation_speed.second.second;
	}

}

void AssetsManager::LoadEffect()
{
	using namespace std;
	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓

	// エフェクトファイル名等を読み込み
	shared_ptr<EffectFileList> effect_file_list = DataBase::Instance().GetEffectFileList();

	// エフェクトファイルの読み込み
	for (const auto& effect_file : effect_file_list->EffectFiles)
	{
		if (effectResource[effect_file.first] == nullptr)
		{
			effectResource[effect_file.first] = make_shared <EffectResource>();
		}
		effectResource[effect_file.first]->ImageNum = effect_file.second.num;
		effectResource[effect_file.first]->Handles = new int[effect_file.second.num];
		LoadDivGraph(
			effect_file.second.fileName,
			effect_file.second.num,
			effect_file.second.xNum,
			effect_file.second.yNum,
			effect_file.second.width,
			effect_file.second.height,
			effectResource[effect_file.first]->Handles
		);
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
		if (vertexShaderResource[vertex_shader_file.first] == nullptr)
		{
			vertexShaderResource[vertex_shader_file.first] = make_shared<VertexShaderResource>();
		}
		vertexShaderResource[vertex_shader_file.first]->Handle = LoadVertexShader(vertex_shader_file.second);
	}

	// ピクセルシェーダー読み込み
	for (const auto& pixel_shader_file : shader_file_list->PixelShaderFiles)
	{
		if (pixelShaderResource[pixel_shader_file.first] == nullptr)
		{
			pixelShaderResource[pixel_shader_file.first] = make_shared<PixelShaderResource>();
		}
		pixelShaderResource[pixel_shader_file.first]->Handle = LoadPixelShader(pixel_shader_file.second);
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