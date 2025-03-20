#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneFactory.h"
#include "../Systems/Time.h"
#include "../Systems/TimerManager.h"
#include "../Input/InputManager.h"
#include <DxLib.h>

SceneManager::SceneManager(std::shared_ptr<SceneBase> initial_scene_) :
	currentScene(initial_scene_)
{

}

SceneManager::~SceneManager()
{

}

bool SceneManager::IsRunning()
{
	return currentScene != nullptr;
}

bool SceneManager::CheckSceneStep()
{
	if (currentScene == nullptr) return false;
	if (currentScene->GetCurrentStep() == SceneBase::Step::Finish)
	{
		// �V�[���I��
		SceneBase::Type next_scene_type = currentScene->Delete();
		currentScene = SceneFactory::Create(next_scene_type);
	}

	if (currentScene == nullptr) return false;
	if (currentScene->GetCurrentStep() == SceneBase::Step::Init)
	{
		// �V�[������
		currentScene->Initialize();
		return true;
	}
	return false;
}

void SceneManager::Main(float elapsed_time_)
{

	TimerManager::Instance().Update(elapsed_time_ * Time::TimeScale);

	FixedUpdate(elapsed_time_ * Time::TimeScale);

	InputManager::Instance().CheckInput();

	Update(elapsed_time_ * Time::TimeScale);

	Render();

}

void SceneManager::FixedUpdate()
{
	if (currentScene == nullptr) return;
	currentScene->FixedUpdate();
#ifdef DEBUG
	fixedNum++;
#endif
	excess = fixedUpdateTimer->GetRemainingTime();
	fixedUpdateTimer = nullptr;
}

void SceneManager::FixedUpdate(float elapsed_time_)
{
	if (fixedUpdateTimer == nullptr)
	{
		fixedUpdateTimer = std::make_unique<Timer<SceneManager>>(Timer<SceneManager>(Time::FixedDeltaTime + excess, this, &SceneManager::FixedUpdate));
	}
	fixedUpdateTimer->Update(elapsed_time_);
}

void SceneManager::Update(float elapsed_time_)
{
	if (currentScene == nullptr) return;
	currentScene->Update(elapsed_time_);

#ifdef DEBUG
	num++;
	if (debugTimer == nullptr)
	{
		debugTimer = std::make_unique<Timer<SceneManager>>(Timer<SceneManager>(1.0f, this, &SceneManager::DebugView));
	}
	debugTimer->Update(elapsed_time_ * Time::TimeScale);
#endif
}

void SceneManager::Render()
{
	ClearDrawScreen();

	if (currentScene == nullptr) return;
	currentScene->Render();

#ifdef DEBUG
	DrawFormatString(0, 10, GetColor(255, 255, 255), "FixedFPS : %d", fixedNumView);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "FPS      : %d", numView);
#endif
	ScreenFlip();
}


#ifdef DEBUG
void SceneManager::DebugView()
{
	numView = num;
	fixedNumView = fixedNum;
	num = 0;
	fixedNum = 0;
	debugTimer = nullptr;
}
#endif