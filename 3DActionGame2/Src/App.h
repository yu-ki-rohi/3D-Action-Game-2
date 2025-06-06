#pragma once
#include <DxLib.h>
#include <memory>

class SceneManager;

class App
{
public:

	App();
	~App();
	
	void Run();

private:
	// 初期化処理
	bool InitApp();
	// 終了処理
	void TermApp();

	// メインループ
	void MainLoop();

private:
	// システム時間
	// 経過時間取得用
	LONGLONG systemTime = GetNowHiPerformanceCount();
	std::unique_ptr<SceneManager> sceneManager;


};