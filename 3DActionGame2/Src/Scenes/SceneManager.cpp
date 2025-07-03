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
		TimerManager::Instance().EraseAll();
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
	if (currentScene == nullptr) return;

	// �V�K�ɒǉ����ꂽObject�̊J�n����
	currentScene->Start();

	// �^�C�}�[�X�V
	TimerManager::Instance().Update(elapsed_time_ * Time::TimeScale);
#ifdef DEBUG
	// VSync - FixedUpdate �̊Ԃ̏������Ԃ��L�^
	profiler.Stamp(Profiler::Type::Other);
#endif

	// �Œ�t���[���̍X�V����
	FixedUpdate(elapsed_time_ * Time::TimeScale);

	// �R���g���[���[�̓��͏󋵊m�F
	InputManager::Instance().CheckInput();

#ifdef DEBUG
	// Input �̏������Ԃ��L�^
	profiler.Stamp(Profiler::Type::Input);
#endif

	// �t���[�����̍X�V����
	Update(elapsed_time_);

	// �`�揈��
	Render();

}

void SceneManager::FixedUpdate()
{
	currentScene->FixedUpdate();

#ifdef DEBUG
	fixedNum++;
#endif           
	excess = fixedUpdateTimer->GetRemainingTime();
}

void SceneManager::FixedUpdate(float elapsed_time_)
{
	if (currentScene->GetCurrentStep() != SceneBase::Step::Update) { return; }
	if (fixedUpdateTimer == nullptr || !fixedUpdateTimer->IsActive())
	{
		fixedUpdateTimer = std::make_unique<Timer<SceneManager,SceneBase>>(Timer<SceneManager, SceneBase>(Time::FixedDeltaTime + excess, currentScene, this, &SceneManager::FixedUpdate));
	}
	fixedUpdateTimer->Update(elapsed_time_);

#ifdef DEBUG
	profiler.Stamp(Profiler::Type::FixedUpdate);
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
	profiler.Stamp(Profiler::Type::Update);

	num++;
	if (debugTimer == nullptr || !debugTimer->IsActive())
	{
		debugTimer = std::make_unique<Timer<SceneManager, SceneBase>>(Timer<SceneManager, SceneBase>(1.0f, currentScene, this, &SceneManager::DebugView));
	}
	debugTimer->Update(elapsed_time_);
#endif
}

void SceneManager::Render()
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
	
#ifdef DEBUG
	profiler.Render();

	DrawFormatString(0, 10, GetColor(255, 255, 255), "FixedFPS : %d", fixedNumView);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "FPS      : %d", numView);
	profiler.Stamp(Profiler::Type::Render);
#endif
	ScreenFlip();
#ifdef DEBUG
	profiler.Stamp(Profiler::Type::VSync);
#endif
}


#ifdef DEBUG
void SceneManager::DebugView()
{
	profiler.Update();
	profiler.ResetTimes();
	numView = num;
	fixedNumView = fixedNum;
	num = 0;
	fixedNum = 0;
}
#endif