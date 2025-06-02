#include "TitleScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Input/InputManager.h"
#include "../Systems/TimerManager.h"

TitleScene::TitleScene() :
	objectManager(std::make_shared<ObjectManager>()),
	objectFactory(std::make_shared<ObjectFactory>(objectManager))
{

}

TitleScene::~TitleScene()
{

}


void TitleScene::FixedUpdate()
{
	if (objectManager != nullptr)
	{
		objectManager->FixedUpdate();
	}
}

void TitleScene::TitleScene::Update(float elapsed_time_)
{
	if (objectManager != nullptr)
	{
		objectManager->Update(elapsed_time_);
	}
}

void TitleScene::TitleScene::Render()
{
	if (objectManager != nullptr)
	{
		objectManager->Render();
	}
}

void TitleScene::TitleScene::Initialize()
{
	if (objectFactory != nullptr)
	{
		// objectFactory->Create();
	}
	currentStep = Step::Update;
}

SceneBase::Type TitleScene::TitleScene::Delete()
{
	InputManager::Instance().Clear();
	TimerManager::Instance().EraseAll();
	return nextType;
}
