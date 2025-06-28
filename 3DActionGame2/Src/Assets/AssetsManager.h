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

class AssetsManager
{
public:
	AssetsManager();
	~AssetsManager();

public:
	// Asset�̓ǂݍ���
	void Load();
	// Asset�̍폜
	void DeleteAll();

	const std::shared_ptr<ModelResource> GetModel(MKind kind_);
	const std::shared_ptr<AnimationResource> GetAnimation(MKind kind_);
	const std::shared_ptr<EffectResource> GetEffect(EKind kind_);
	const std::shared_ptr<VertexShaderResource> GetVertexShader(VSKind kind_);
	const std::shared_ptr<PixelShaderResource> GetPixelShader(PSKind kind_);

	int GetTmpScreenHandle();
	int GetShadowMapHandle();

private:
	void LoadModel();
	void LoadAnimation();
	void LoadEffect();
	void LoadShader();
	void MakeTmpScreen();
	void MakeShadowMap();

private:
	std::unordered_map<MKind, std::shared_ptr<ModelResource>> modelResource;
	std::unordered_map<MKind, std::shared_ptr<AnimationResource>> animationResource;
	std::unordered_map<EKind, std::shared_ptr<EffectResource>> effectResource;
	std::unordered_map<VSKind, std::shared_ptr<VertexShaderResource>> vertexShaderResource;
	std::unordered_map<PSKind, std::shared_ptr<PixelShaderResource>> pixelShaderResource;

	// �|�X�g�G�t�F�N�g�������邽�߂ɁA�ꎟ�I�ɕ`�����ރX�N���[���̃n���h��
	// �����Ɨǂ��������Ȃ�������
	int tmpScreenHandle;

	// �V���h�E�}�b�v
	int shadowMapHandle;
};