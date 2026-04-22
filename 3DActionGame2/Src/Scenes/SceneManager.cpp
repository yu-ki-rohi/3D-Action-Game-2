#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneFactory.h"
#include "../Systems/TimerManager.h"
#include "../Input/InputManager.h"
#include "../Debug/DebugManager.h"
#include <DxLib.h>

#define FIXEDUPDATE_LOOP

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
		// シーン終了
		TimerManager::Instance().EraseAll();
		SceneBase::Type next_scene_type = currentScene->Delete();
		currentScene = SceneFactory::Create(next_scene_type);
	}

	if (currentScene == nullptr) return false;
	if (currentScene->GetCurrentStep() == SceneBase::Step::Init)
	{
		// シーン準備
		currentScene->Initialize();
		return true;
	}
	return false;
}

void SceneManager::Main(float elapsed_time_)
{
	if (currentScene == nullptr) return;

	// 新規に追加されたObjectの開始処理
	currentScene->Start();

	// タイマー更新
	TimerManager::Instance().Entry();
	TimerManager::Instance().Update(elapsed_time_ * Time::TimeScale);

#ifdef DEBUG
#ifdef DELAY_EMULATE
	for (long i = 0; i < delayLoopNum; ++i)
	{
		VECTOR vec = VGet(i, -i, i / 3.0f);
		vec = VNorm(vec);
	}
#endif
	// VSync - FixedUpdate の間の処理時間を記録
	DebugManager::Instance().Stamp(Profiler::Type::Other);
#endif

	// 固定フレームの更新処理
	FixedUpdate(elapsed_time_ * Time::TimeScale);

	// コントローラーの入力状況確認
	InputManager::Instance().CheckInput();

#ifdef DEBUG
	// Input の処理時間を記録
	DebugManager::Instance().Stamp(Profiler::Type::Input);

	DebugManager::Instance().Update(elapsed_time_);
#endif

	// フレーム毎の更新処理
	Update(elapsed_time_);

	// 描画処理
	Render(elapsed_time_);

}

void SceneManager::FixedUpdate(float elapsed_time_)
{
	if (currentScene->GetCurrentStep() != SceneBase::Step::Update) { return; }

	elapsedTimeSinceLastFixedUpdate += elapsed_time_;

#ifdef FIXEDUPDATE_LOOP
	int loopNum = (int)(elapsedTimeSinceLastFixedUpdate * Time::FixedFrame);
	for (int i = 0; i < loopNum; ++i)
	{
		currentScene->FixedUpdate();

#ifdef DEBUG
		fixedNum++;
#endif
	}
	elapsedTimeSinceLastFixedUpdate -= Time::FixedDeltaTime * loopNum;

#else
	if (elapsedTimeSinceLastFixedUpdate >= Time::FixedDeltaTime)
	{
		currentScene->FixedUpdate();
		elapsedTimeSinceLastFixedUpdate -= Time::FixedDeltaTime;
#ifdef DEBUG
		fixedNum++;
#endif
	}


#endif
#ifdef DEBUG

	DebugManager::Instance().Stamp(Profiler::Type::FixedUpdate);
#endif       
}

void SceneManager::Update(float elapsed_time_)
{
	if (currentScene->GetCurrentStep() == SceneBase::Step::Load)
	{
		currentScene->UpdateInLoading(elapsed_time_ * Time::TimeScale);
	}
	else if (currentScene->GetCurrentStep() == SceneBase::Step::Update)
	{
		currentScene->Update(elapsed_time_ * Time::TimeScale);
	}
	
#ifdef DEBUG
	DebugManager::Instance().Stamp(Profiler::Type::Update);

	num++;
	if (debugTimer == nullptr || !debugTimer->IsActive())
	{
		debugTimer = std::make_unique<Timer<SceneManager, SceneBase>>(Timer<SceneManager, SceneBase>(1.0f, currentScene, this, &SceneManager::DebugView));
	}
	debugTimer->Update(elapsed_time_);
#endif
}

void SceneManager::Render(float elapsed_time_)
{
	elapsedTimeSinceLastRenderUpdate += elapsed_time_;
	if (elapsedTimeSinceLastRenderUpdate >= Time::MinSPF)
	{
		ClearDrawScreen();

		if (currentScene->GetCurrentStep() == SceneBase::Step::Load)
		{
			currentScene->RenderInLoading();
		}
		else if (currentScene->GetCurrentStep() == SceneBase::Step::Update)
		{
			currentScene->Render();
		}

		elapsedTimeSinceLastRenderUpdate -= Time::MinSPF;
#ifdef DEBUG

		DebugManager::Instance().Render();

		DrawFormatString(10, 20, GetColor(255, 255, 255), "Logic Update : %d", numView);
		DrawFormatString(10, 40, GetColor(255, 255, 255), "Fixed Update : %d", fixedNumView);
		DrawFormatString(10, 60, GetColor(255, 255, 255), "FPS          : %d", fpsView);

		DebugManager::Instance().Stamp(Profiler::Type::Render);
		fps++;
#endif	
		ScreenFlip();
#ifdef DEBUG
		DebugManager::Instance().Stamp(Profiler::Type::VSync);
#endif
	}

#ifdef DEBUG
	
#endif

}


#ifdef DEBUG
void SceneManager::DebugView()
{
	DebugManager::Instance().UpdateProfiler();
	DebugManager::Instance().ResetTimes();
	numView = num;
	fixedNumView = fixedNum;
	fpsView = fps;
	num = 0;
	fixedNum = 0;
	fps = 0;
}
#endif