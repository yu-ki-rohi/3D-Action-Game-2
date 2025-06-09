#pragma once
#include "SceneBase.h"
#include <memory>
#include "../Common.h"
#include <DxLib.h>

class ObjectManager;
class ObjectFactory;
class SampleCamera;
class SampleCharacter;

class SampleScene : public SceneBase
{
public:
	SampleScene();
	~SampleScene();

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
	void Load();
	void DeleteHandles();

	void IncreaseIntensity();
	void DecreaseIntensity();

	void SetupDepthImage();

private:
	std::shared_ptr<ObjectManager> objectManager;
	std::shared_ptr<ObjectFactory> objectFactory;
	Type nextType = Type::Game;

	int stageHandle = -1;
	int charaHandle = -1;
	int idleHandle = -1;
	int walkHandle = -1;
	int attackHandle = -1;
	int enemyHandle = -1;

	int normalVertexShaderHandle = -1;
	int skinnedVertexShaderHandle = -1;
	int normalPixelShaderHandle = -1;
	int skinnedPixelShaderHandle = -1;
	int pixelShaderHandle = -1;

	int normalShadowMapVSHandle = -1;
	int skinnedShadowMapVSHandle = -1;
	int shadowMapPSHandle = -1;

	int blurPSHandle = -1;

	int tmpScreenHandle = -1;

	int shadowMapHandle00 = -1;

	float intensity = 0.0f;

	std::shared_ptr<SampleCamera> sampleCamera;
	std::shared_ptr<SampleCharacter> sampleChara;

	MATRIX lightCameraViewMatrix;
	MATRIX lightCameraProjectionMatrix;

#ifdef DEBUG
	int maxMeshNum = 0;
#endif
};