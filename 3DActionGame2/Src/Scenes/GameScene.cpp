#include "GameScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/ColliderRegisterInterface.h"

GameScene::GameScene() :
	objectManager(std::make_shared<ObjectManager>()),
	objectFactory(std::make_shared<ObjectFactory>(objectManager)),
	collisionManager(std::make_shared<CollisionManager>()),
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

		isReady = true;
	}
}

void GameScene::RenderInLoading()
{

}

void GameScene::Initialize()
{

	currentStep = Step::Load;
}

SceneBase::Type GameScene::Delete()
{

}

void GameScene::SetupLight()
{

}
void GameScene::ASyncLoad()
{
	SetUseASyncLoadFlag(TRUE);
	// «««”ñ“¯Šú“Ç‚İ‚İŠÖ”‚Í‚±‚±‚©‚ç‰º‚ÅŒÄ‚Ô«««


	// ªªª”ñ“¯Šú“Ç‚İ‚İŠÖ”‚Í‚±‚±‚©‚çã‚ÅŒÄ‚Ôªªª
	SetUseASyncLoadFlag(FALSE);
	aSyncLoadNumMax = GetASyncLoadNum();
}
void GameScene::DeleteHandles()
{

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