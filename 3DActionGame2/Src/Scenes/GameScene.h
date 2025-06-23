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

	// 描画は以下を参考に作っています
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

	// デプスシャドウ描画の準備
	void SetupDepthImage();

	void PostProcessing();

	void GenerateObjects();

	void FinishJustAvoid();

private:
	// オブジェクトの管理クラス
	std::shared_ptr<ObjectManager> objectManager;

	// 当たり判定を管理クラス
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetsManager> assetsManager;

	std::shared_ptr<CameraManager> cameraManager;

	// Colliderの登録・解除を行うインターフェース
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;
	// オブジェクト生成クラス
	std::shared_ptr<ObjectFactory> objectFactory;

	std::shared_ptr<PlayerEventNotifier> playerEventNotifier;

	bool isReady;
	int aSyncLoadNumMax;

	bool isJustAvoidTime;
	// ラディアルブラーをかける度合
	float intensity;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};