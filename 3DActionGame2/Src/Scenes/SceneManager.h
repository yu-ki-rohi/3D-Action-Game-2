#pragma once
#include <memory>
#include "../Systems/Timer.h"
#include "../Common.h"
#include "../Debug/Profiler.h"
#include "../Systems/Time.h"

//#define DELAY_EMULATE


class SceneBase;


// Sceneを運営するクラス
// 基本的な作りは先生から頂いたソースに基づいている

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
	void FixedUpdate(float elapsed_time_);

	void Update(float elapsed_time_);

	void Render(float elapsed_time_);

#ifdef DEBUG
	void DebugView();
#endif

private:
	// 現在のシーン
	std::shared_ptr<SceneBase> currentScene;

	// FixedUpdateを実行させる時間を管理する値
	float elapsedTimeSinceLastFixedUpdate = 0.0f;
	float elapsedTimeSinceLastRenderUpdate = 0.0f;

	// 超過時間(次回FixedUpdateまでの補正のため)
	float excess = 0.0f;

	float renderExcess = Time::MinSPF;

#ifdef DEBUG
	// テストのために一旦こちらで直接保持
	// todo: 最終的にはDebugManagerにまとめる
	//Profiler profiler;

	int num = 0;
	int numView = 0;
	int fixedNum = 0;
	int fixedNumView = 0;
	int fps = 0;
	int fpsView = 0;
	std::unique_ptr<Timer<SceneManager, SceneBase>> debugTimer = nullptr;

#ifdef DELAY_EMULATE
	static constexpr long delayLoopNum = 150000;
#endif
#endif
};