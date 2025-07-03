#pragma once
#include <memory>
#include <unordered_map>
#include "../DataBase/DataKind.h"

// Assets
// handle
// 
// Data
// file, path
// prameters
//

struct ModelResource;
struct AnimationResource;
struct EffectResource;
struct VertexShaderResource;
struct PixelShaderResource;

struct MusicResource;
struct SoundResource;


// 主にHandleを管理させるクラス
class AssetsManager
{
public:
	AssetsManager();
	~AssetsManager();

public:
	// Assetの読み込み
	void Load();
	// Assetの削除
	void DeleteAll();

	const std::shared_ptr<ModelResource> GetModel(MKind kind_);
	const std::shared_ptr<AnimationResource> GetAnimation(MKind kind_);
	const std::shared_ptr<EffectResource> GetEffect(EKind kind_);
	const std::shared_ptr<VertexShaderResource> GetVertexShader(VSKind kind_);
	const std::shared_ptr<PixelShaderResource> GetPixelShader(PSKind kind_);

	int GetTmpScreenHandle() const;
	int GetShadowMapHandle() const;

	void PrepareToPlayAudio();

private:
	void LoadModel();
	void LoadAnimation();
	void LoadEffect();
	void LoadShader();
	void MakeTmpScreen();
	void MakeShadowMap();

	void LoadAudio();

private:
	// Assets
	std::unordered_map<MKind, std::shared_ptr<ModelResource>> modelResource;
	std::unordered_map<MKind, std::shared_ptr<AnimationResource>> animationResource;
	std::unordered_map<EKind, std::shared_ptr<EffectResource>> effectResource;
	std::unordered_map<VSKind, std::shared_ptr<VertexShaderResource>> vertexShaderResource;
	std::unordered_map<PSKind, std::shared_ptr<PixelShaderResource>> pixelShaderResource;

	// ポストエフェクトをかけるために、一次的に描きこむスクリーンのハンドル
	// もっと良い命名がないか検討
	int tmpScreenHandle;

	// シャドウマップ
	int shadowMapHandle;

	// 管理権限をAudioManagerにも渡すので、shared_ptrにする
	std::shared_ptr<std::unordered_map<BGMKind, std::shared_ptr<MusicResource>>> musicResource;
	std::shared_ptr<std::unordered_map<SEKind, std::shared_ptr<SoundResource>>> soundResource;

};