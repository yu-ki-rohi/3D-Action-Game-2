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
class PlayerEventNotifier;

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene();

public:
	bool IsJustAvoidTime();

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

public:
	static constexpr float JustAvoidLocalTimeScale = 0.05f;

private:
	void SetupLight();
	void ASyncLoad();

	// デプスシャドウ描画の準備
	void SetupDepthImage();

	void PostProcessing();

	void GenerateObjects();

	void ResetTimeScale();
	void FinishJustAvoidEffect();
	void FinishJustAvoidTime();

	void ReturnTitle();

private:
	// オブジェクトの管理クラス
	std::shared_ptr<ObjectManager> objectManager;

	// 当たり判定を管理クラス
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetsManager> assetsManager;

	std::shared_ptr<CameraManager> cameraManager;

	// オブジェクト生成クラス
	std::shared_ptr<ObjectFactory> objectFactory;

	std::shared_ptr<PlayerEventNotifier> playerEventNotifier;

	// ロード用

	bool isReady;
	int aSyncLoadNumMax;

	// ジャスト回避のボーナス状態かどうか
	bool isJustAvoidTime;

	// ラディアルブラー関係の変数
	
	// 現在のラディアルブラーをかける度合
	float intensity;
	// ラディアルブラーをかける度合いの目標値
	float targetIntensity;
	// ラディアルブラーの変動スピード
	float fluctuationSpeedOfIntensity;

	int monochromeMask;

	// モノクロ描画関係の変数

	// 現在のモノクロ割合
	float monochromeRate;
	// 目標のモノクロ割合
	float targetMonochromeRate;
	// モノクロ割合の変動スピード
	float fluctuationSpeedOfMonochromeRate;
	// モノクロ変動させるマスク

	// 影描画用

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;
};