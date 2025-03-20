#include "App.h"
#include <time.h>
#include "Systems/SystemTimeProcess.h"
#include "Systems/Time.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneFactory.h"

App::App()
{

}

App::~App()
{

}

void App::Run()
{
	if (InitApp())
	{
		MainLoop();
	}
	TermApp();
}

bool App::InitApp()
{
	// �����̐�������
	srand((unsigned int)time(NULL));
	// DXLib�������O�̎��O����
	ChangeWindowMode(true);
	int window_width = 1920;
	int window_height = 1080;
	int color_bit = 32;
	SetGraphMode(window_width, window_height, color_bit);

	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	SetJoypadDeadZone(DX_INPUT_PAD1, 0.3f);
	SetDrawScreen(DX_SCREEN_BACK);

	sceneManager = std::make_unique<SceneManager>(SceneFactory::Create(SceneBase::Type::Sample));

	systemTime = GetNowHiPerformanceCount();

	return true;
}

void App::TermApp()
{
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

}

void App::MainLoop()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (sceneManager == nullptr) break;

		// �V�[���̏������E�I������
		if (sceneManager->CheckSceneStep())
		{
			// �����������s���ꂽ���1�t���[���ڂɈُ�l��n���Ȃ�����
			SystemTimeProcess::GetElapsedTime(systemTime);
		}
		// �A�v���I���m�F
		if (!sceneManager->IsRunning()) break;

		// �o�ߎ��Ԏ擾
		float elapsed_time = SystemTimeProcess::GetElapsedTime(systemTime);

		// ���C������
		sceneManager->Main(elapsed_time);
	}
}
