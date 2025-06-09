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

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene();

public:
	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void UpdateInLoading(float elapsed_time_) override {};
	void RenderInLoading() override {};

	void Initialize() override;
	Type Delete() override;

private:
	void SetupLight();
	void ASyncLoad();
	void DeleteHandles();

	void IncreaseIntensity();
	void DecreaseIntensity();

	// �f�v�X�V���h�E�`��̏���
	void SetupDepthImage();


	void SetMonochrome(float rate_);
	void SetMonochrome(float rate_, ObjectBase::Tag type_);


private:
	// �I�u�W�F�N�g�̊Ǘ�
	std::shared_ptr<ObjectManager> objectManager;
	// �I�u�W�F�N�g����
	std::shared_ptr<ObjectFactory> objectFactory;
	// �����蔻����Ǘ�
	std::shared_ptr<CollisionManager> collisionManager;
	// Collider�̓o�^�E�������s���C���^�[�t�F�[�X
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	bool isReady;
	int aSyncLoadNumMax;

	// ���f�B�A���u���[��������x��
	float intensity;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};