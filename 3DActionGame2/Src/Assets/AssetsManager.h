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
	const std::shared_ptr<VertexShaderResource> GetVertexShader(VSKind kind_);
	const std::shared_ptr<PixelShaderResource> GetPixelShader(PSKind kind_);

	int GetTmpScreenHandle();
	int GetShadowMapHandle();

private:
	void LoadModel();
	void LoadAnimation();
	void LoadShader();
	void MakeTmpScreen();
	void MakeShadowMap();

private:
	std::unordered_map<MKind, std::shared_ptr<ModelResource>> modelHandles;
	std::unordered_map<MKind, std::shared_ptr<AnimationResource>> animationHandles;
	std::unordered_map<VSKind, std::shared_ptr<VertexShaderResource>> vertexShaderHandles;
	std::unordered_map<PSKind, std::shared_ptr<PixelShaderResource>> pixelShaderHandles;

	// �|�X�g�G�t�F�N�g�������邽�߂ɁA�ꎟ�I�ɕ`�����ރX�N���[���̃n���h��
	// �����Ɨǂ��������Ȃ�������
	int tmpScreenHandle;

	// �V���h�E�}�b�v
	int shadowMapHandle;
};