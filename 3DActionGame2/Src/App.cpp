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
	// 乱数の生成準備
	srand((unsigned int)time(NULL));
	// DXLib初期化前の事前準備
	ChangeWindowMode(true);
	int window_width = 1920;
	int window_height = 1080;
	int color_bit = 32;
	SetGraphMode(window_width, window_height, color_bit);

	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return false;			// エラーが起きたら直ちに終了
	}

	SetJoypadDeadZone(DX_INPUT_PAD1, 0.3f);
	SetDrawScreen(DX_SCREEN_BACK);

	sceneManager = std::make_unique<SceneManager>(SceneFactory::Create(SceneBase::Type::Sample));

	systemTime = GetNowHiPerformanceCount();

	return true;
}

void App::TermApp()
{
	DxLib_End();				// ＤＸライブラリ使用の終了処理

}

void App::MainLoop()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (sceneManager == nullptr) break;

		// シーンの初期化・終了処理
		if (sceneManager->CheckSceneStep())
		{
			// 初期化が実行された後の1フレーム目に異常値を渡さないため
			SystemTimeProcess::GetElapsedTime(systemTime);
		}
		// アプリ終了確認
		if (!sceneManager->IsRunning()) break;

		// 経過時間取得
		float elapsed_time = SystemTimeProcess::GetElapsedTime(systemTime);

		// メイン処理
		sceneManager->Main(elapsed_time);
	}
}
