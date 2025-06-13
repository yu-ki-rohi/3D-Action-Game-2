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

	// デプスシャドウ描画の準備
	void SetupDepthImage();

	void GenerateObjects();

	void SetMonochrome(float rate_);
	void SetMonochrome(float rate_, ObjectBase::Tag type_);


private:
	// オブジェクトの管理クラス
	std::shared_ptr<ObjectManager> objectManager;

	// 当たり判定を管理クラス
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetsManager> assetsManager;

	// オブジェクト生成クラス
	std::shared_ptr<ObjectFactory> objectFactory;
	// Colliderの登録・解除を行うインターフェース
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	bool isReady;
	int aSyncLoadNumMax;

	// ラディアルブラーをかける度合
	float intensity;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};