#include "GameScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/ColliderRegisterInterface.h"
#include "../Assets/AssetsManager.h"
#include "../Common.h"

GameScene::GameScene() :
	objectManager(std::make_shared<ObjectManager>()),
	collisionManager(std::make_shared<CollisionManager>()),
	assetsManager(std::make_shared<AssetsManager>()),
	objectFactory(std::make_shared<ObjectFactory>(objectManager, assetsManager)),
	colliderRegisterInterface(std::make_shared<ColliderRegisterInterface>(collisionManager)),
	isReady(false),
	aSyncLoadNumMax(100),
	intensity(0.0f),
	lightCameraViewMatrix(MGetIdent()),
	lightCameraProjectionMatrix(MGetIdent())
{

}

GameScene::~GameScene()
{

}

void GameScene::FixedUpdate()
{
	if (collisionManager != nullptr)
	{
		collisionManager->CheckCollision();
	}

	if (objectManager != nullptr)
	{
		objectManager->FixedUpdate();
	}
}

void GameScene::Update(float elapsed_time_)
{
	if (objectManager != nullptr)
	{
		objectManager->Update(elapsed_time_);
	}
}

void GameScene::Render()
{
	SetupDepthImage();

	if (objectManager != nullptr)
	{
		objectManager->RenderShadow();
	}
}

void GameScene::UpdateInLoading(float elapsed_time_)
{
	if (isReady)
	{

	}
	else if (GetASyncLoadNum() == 0)
	{
		GenerateObjects();
		isReady = true;
	}
}

void GameScene::RenderInLoading()
{
	const int width = WindowSettings::WindowWidth * 0.8f;
	const int height = WindowSettings::WindowHeight * 0.1f;
	int x = WindowSettings::WindowWidth * 0.1f;
	int y = WindowSettings::WindowHeight * 0.45f;
	unsigned int color = GetColor(255, 255, 255);
	float rate = 1.0f;
	if (aSyncLoadNumMax != 0)
	{
		rate = (aSyncLoadNumMax - GetASyncLoadNum()) / (float)aSyncLoadNumMax;
	}
	DrawBox(x, y, x + width * rate, y + height, color, TRUE);
}

void GameScene::Initialize()
{

	currentStep = Step::Load;
}

SceneBase::Type GameScene::Delete()
{
	return Type::Title;
}

void GameScene::SetupLight()
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

void GameScene::ASyncLoad()
{
	SetUseASyncLoadFlag(TRUE);
	// ↓↓↓非同期読み込み関数はここから下で呼ぶ↓↓↓

	assetsManager->Load();

	// ↑↑↑非同期読み込み関数はここから上で呼ぶ↑↑↑
	SetUseASyncLoadFlag(FALSE);
	aSyncLoadNumMax = GetASyncLoadNum();
}

void GameScene::IncreaseIntensity()
{

}
void GameScene::DecreaseIntensity()
{

}

void GameScene::SetupDepthImage() 
{

}

void GameScene::GenerateObjects()
{

}

void GameScene::SetMonochrome(float rate_)
{

}

void GameScene::SetMonochrome(float rate_, ObjectBase::Tag type_)
{

}