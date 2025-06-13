#pragma once
#include "SceneBase.h"
#include <memory>
#include "../Common.h"
#include <DxLib.h>
#include "../Objects/ObjectBase.h"

class ObjectManager;
class ObjectFactory;
class CollisionManager;
class ColliderRegisterInterface;
class AssetsManager;

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene();

public:
	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void UpdateInLoading(float elapsed_time_) override;
	void RenderInLoading() override;

	void Initialize() override;
	Type Delete() override;

private:
	void SetupLight();
	void ASyncLoad();

	void IncreaseIntensity();
	void DecreaseIntensity();

	// �f�v�X�V���h�E�`��̏���
	void SetupDepthImage();

	void GenerateObjects();

	void SetMonochrome(float rate_);
	void SetMonochrome(float rate_, ObjectBase::Tag type_);


private:
	// �I�u�W�F�N�g�̊Ǘ��N���X
	std::shared_ptr<ObjectManager> objectManager;

	// �����蔻����Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetsManager> assetsManager;

	// �I�u�W�F�N�g�����N���X
	std::shared_ptr<ObjectFactory> objectFactory;
	// Collider�̓o�^�E�������s���C���^�[�t�F�[�X
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	bool isReady;
	int aSyncLoadNumMax;

	// ���f�B�A���u���[��������x��
	float intensity;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};