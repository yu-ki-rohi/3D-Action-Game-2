#pragma once
#include <memory>
#include "../Systems/Timer.h"
#include "../Common.h"
#include "../Debug/Profiler.h"

#define VSYNC

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
	/// Sceneの段階を確認
	/// </summary>
	/// <returns>初期化が実行されたかどうか</returns>
	bool CheckSceneStep();

	// シーンの実行
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
	// 現在のシーン
	std::shared_ptr<SceneBase> currentScene;

	// FixedUpdateを実行させるためのタイマー
	// SceneManagerは存在しないことがまずないので、特例でダミーのsharedとしてSceneBaseを渡しておく
	std::unique_ptr<Timer<SceneManager, SceneBase>> fixedUpdateTimer = nullptr;
	std::unique_ptr<Timer<SceneManager, SceneBase>> renderUpdateTimer = nullptr;

	// 超過時間(次回FixedUpdateまでの補正のため)
	float excess = 0.0f;


#ifdef DEBUG
	// テストのために一旦こちらで直接保持
	// TODO
	// 最終的にはDebugManagerにまとめる
	Profiler profiler;

	int num = 0;
	int numView = 0;
	int fixedNum = 0;
	int fixedNumView = 0;
	std::unique_ptr<Timer<SceneManager>> debugTimer = nullptr;
#endif
};