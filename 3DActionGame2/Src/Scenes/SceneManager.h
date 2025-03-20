#pragma once
#include <memory>
#include "../Systems/Timer.h"
#include "../Common.h"


class SceneBase;

class SceneManager
{
public:
	SceneManager(std::shared_ptr<SceneBase> initial_scene_);
	~SceneManager();

public:
	// アプリ終了用
	bool IsRunning();
public:
	/// <summary>
	/// 
	/// </summary>
	/// <returns>初期化が実行されたかどうか</returns>
	bool CheckSceneStep();
	void Main(float elapsed_time_);

private:
	// Timerで呼び出す
	void FixedUpdate();
	// MainLoop内で呼び出す
	void FixedUpdate(float elapsed_time_);

	void Update(float elapsed_time_);

	void Render();

#ifdef DEBUG
	void DebugView();
#endif

private:
	std::shared_ptr<SceneBase> currentScene;

	std::unique_ptr<Timer<SceneManager>> fixedUpdateTimer = nullptr;
	float excess = 0.0f; // 超過時間


#ifdef DEBUG
	int num = 0;
	int numView = 0;
	int fixedNum = 0;
	int fixedNumView = 0;
	std::unique_ptr<Timer<SceneManager>> debugTimer = nullptr;
#endif
};