#include "SampleScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Objects/SampleCharacter.h"
#include "../Objects/SampleCamera.h"
#include "../Input/InputManager.h"
#include "../Systems/TimerManager.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Mathmatics/Vector3.h"
#include <DxLib.h>

SampleScene::SampleScene() :
	objectManager(std::make_shared<ObjectManager>()),
	objectFactory(std::make_shared<ObjectFactory>(objectManager))
{
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Trigger::Right,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleScene>>(this, &SampleScene::IncreaseIntensity));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Trigger::Left,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleScene>>(this, &SampleScene::DecreaseIntensity));
}

SampleScene::~SampleScene()
{
	DeleteHandles();
}


void SampleScene::FixedUpdate()
{
	if (objectManager != nullptr)
	{
		objectManager->FixedUpdate();
	}
}

void SampleScene::SampleScene::Update(float elapsed_time_)
{
	if (objectManager != nullptr)
	{
		objectManager->Update(elapsed_time_);
	}
}

void SampleScene::SampleScene::Render()
{
	SetDrawScreen(tmpScreenHandle);
	ClearDrawScreen();

	sampleCamera->SetCameraInfo();

	if (objectManager != nullptr)
	{
		objectManager->Render();
	}

	ResetPSConstF(22, 1);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, tmpScreenHandle, FALSE);

#ifdef DEBUG
	DrawFormatString(0, 50, GetColor(255, 255, 255), "ScreenNum : %d", GetMultiDrawScreenNum());
#endif

}

void SampleScene::SampleScene::Initialize()
{
	Load();

	//SetDrawScreen(tmpScreenHandle);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	SetupLight();

	SetCameraNearFar(0.1f, 2000.0f);


	MV1SetUseOrigShader(TRUE);

	if (objectFactory != nullptr)
	{
		// カメラ作成
		sampleCamera = objectFactory->CreateCamera(Vector3(5.0f, 18.0f, -30.0f));

		// ステージ作成
		objectFactory->Create(Vector3(), stageHandle, normalVertexShaderHandle, normalPixelShaderHandle);

		// キャラ作成
		std::shared_ptr<SampleCharacter> sampleChara = objectFactory->CreateCharacter(Vector3(), Vector3(0.1f, 0.1f, 0.1f), charaHandle, skinnedVertexShaderHandle, skinnedPixelShaderHandle, idleHandle, sampleCamera);
		std::shared_ptr<SampleCharacter> sampleEnemy = objectFactory->CreateCharacter(Vector3(20.0f, 0.0f, 10.0f), Vector3(0.15f, 0.15f, 0.15f), enemyHandle, skinnedVertexShaderHandle, skinnedPixelShaderHandle, idleHandle, nullptr);

		sampleChara->RegisterAnim(Animator::AnimType::Attack00, attackHandle);
		sampleChara->RegisterAnim(Animator::AnimType::WalkF, walkHandle);

		sampleEnemy->RegisterAnim(Animator::AnimType::Attack00, attackHandle);
		sampleEnemy->RegisterAnim(Animator::AnimType::WalkF, walkHandle);

	}

#ifdef DEBUG


#endif

	currentStep = Step::Update;
}

SceneBase::Type SampleScene::SampleScene::Delete()
{
	InputManager::Instance().Clear();
	TimerManager::Instance().EraseAll();

	DeleteHandles();

	return nextType;
}

void SampleScene::SetupLight()
{
	/*
		ライトの使用をONにする
	*/
	SetUseLighting(true);

	/*
		ライティングの設定をONにする
		この設定をONにしないとディレクショナルライト等の計算が行われない
	*/
	SetLightEnable(true);

	/*
		グローバルAmbient(環境光)の設定をする
	*/
	SetGlobalAmbientLight(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));

	/*
		設定されているライトの方向設定
			※デフォルトはディレクショナルライトが設定されている
	*/
	VECTOR light_dir = VGet(0.8f, -0.5f, 0.4f);
	ChangeLightTypeDir(light_dir);

	/*
		ライトの拡散光の色を設定する
	*/
	SetLightDifColor(GetColorF(0.7f, 0.7f, 0.8f, 1.0f));

	/*
		ライトの環境光の色を設定する
	*/
	SetLightAmbColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
}

void SampleScene::Load()
{
	stageHandle = MV1LoadModel("Res/Models/Stage/StageA01.mv1");
	charaHandle = MV1LoadModel("Res/Models/Player/Player_Model.mv1");
	idleHandle = MV1LoadModel("Res/Models/Player/Player_Idle.mv1");
	walkHandle = MV1LoadModel("Res/Models/Player/Player_Walk.mv1");
	attackHandle = MV1LoadModel("Res/Models/Player/Player_Attack1.mv1");
	enemyHandle = MV1LoadModel("Res/Models/Enemy/Enemy_Model.mv1");

	normalVertexShaderHandle = LoadVertexShader("Res/Shader/Test/TestVertex02.vso");
	skinnedVertexShaderHandle = LoadVertexShader("Res/Shader/Test/TestVertex04.vso");
	normalPixelShaderHandle = LoadPixelShader("Res/Shader/Test/TestPixel00.pso");
	skinnedPixelShaderHandle = LoadPixelShader("Res/Shader/Test/TestPixel04.pso");
	blurPSHandle = LoadPixelShader("Res/Shader/Test/RadialBlurPS.pso");

	tmpScreenHandle = MakeScreen(1920, 1080, TRUE);
}

void SampleScene::DeleteHandles()
{
	int handles[] = {
		stageHandle,
		charaHandle,
		idleHandle,
		walkHandle,
		attackHandle,
		enemyHandle
	};

	for (auto handle : handles)
	{
		MV1DeleteModel(handle);
	}

	InitShader();
	
	DeleteGraph(tmpScreenHandle);
}

void SampleScene::IncreaseIntensity()
{
	intensity += 0.01f;
	if (intensity > 1.0f) { intensity = 1.0f; }
}

void SampleScene::DecreaseIntensity()
{
	intensity -= 0.01f;
	if (intensity < 0.0f) { intensity = 0.0f; }
}