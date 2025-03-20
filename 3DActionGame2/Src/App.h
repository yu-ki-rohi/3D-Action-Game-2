#pragma once
#include <DxLib.h>
#include <memory>

class SceneManager;

class App
{
public:

#pragma region コンストラクター・デストラクター
	App();
	~App();
#pragma endregion
	
#pragma region アクセサ

#pragma endregion
	
#pragma region 公開関数
	void Run();
#pragma endregion

#pragma region 非公開関数
private:
	bool InitApp();
	void TermApp();

	void MainLoop();

#pragma endregion

#pragma region 非公開メンバ変数
private:
	LONGLONG systemTime = GetNowHiPerformanceCount();

	std::unique_ptr<SceneManager> sceneManager;


#pragma endregion

};