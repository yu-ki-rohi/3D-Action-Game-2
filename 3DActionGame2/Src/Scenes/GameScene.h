#pragma once
#include "SceneBase.h"
#include "../Common.h"
#include <DxLib.h>
#include "../Objects/ObjectBase.h"

class ObjectManager;
class CollisionManager;
class AssetsManager;
class CameraManager;
class ObjectFactory;
class ColliderRegisterInterface;
class PlayerEventNotifier;

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene();

public:
	void Start() override;

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;

	// �`��͈ȉ����Q�l�ɍ���Ă��܂�
	// https://dxlib.xsrv.jp/program/dxprogram_3DModelShaderBase.html
	// https://dxlib.xsrv.jp/program/dxprogram_3DAction_DepthShadow.html
	void Render() override;

	void UpdateInLoading(float elapsed_time_) override;
	void RenderInLoading() override;

	void Initialize() override;
	Type Delete() override;

public:
	void SuccessJustAvoid() override;

private:
	void SetupLight();
	void ASyncLoad();

	// �f�v�X�V���h�E�`��̏���
	void SetupDepthImage();

	void PostProcessing();

	void GenerateObjects();

	void FinishJustAvoid();

private:
	// �I�u�W�F�N�g�̊Ǘ��N���X
	std::shared_ptr<ObjectManager> objectManager;

	// �����蔻����Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetsManager> assetsManager;

	std::shared_ptr<CameraManager> cameraManager;

	// Collider�̓o�^�E�������s���C���^�[�t�F�[�X
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;
	// �I�u�W�F�N�g�����N���X
	std::shared_ptr<ObjectFactory> objectFactory;

	std::shared_ptr<PlayerEventNotifier> playerEventNotifier;

	bool isReady;
	int aSyncLoadNumMax;

	bool isJustAvoidTime;
	// ���f�B�A���u���[��������x��
	float intensity;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};