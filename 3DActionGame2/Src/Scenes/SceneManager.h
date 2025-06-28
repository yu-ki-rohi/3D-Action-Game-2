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
	// �A�v���I���p
	bool IsRunning();
public:
	/// <summary>
	/// Scene�̒i�K���m�F
	/// </summary>
	/// <returns>�����������s���ꂽ���ǂ���</returns>
	bool CheckSceneStep();

	// �V�[���̎��s
	void Main(float elapsed_time_);

private:
	// Timer�ŌĂяo��
	void FixedUpdate();
	// MainLoop���ŌĂяo��
	void FixedUpdate(float elapsed_time_);

	void Update(float elapsed_time_);

	void Render();

#ifdef DEBUG
	void DebugView();
#endif

private:
	// ���݂̃V�[��
	std::shared_ptr<SceneBase> currentScene;

	// FixedUpdate�����s�����邽�߂̃^�C�}�[
	// SceneManager�͑��݂��Ȃ����Ƃ��܂��Ȃ��̂ŁA����Ń_�~�[��shared�Ƃ���SceneBase��n���Ă���
	std::unique_ptr<Timer<SceneManager, SceneBase>> fixedUpdateTimer = nullptr;
	std::unique_ptr<Timer<SceneManager, SceneBase>> renderUpdateTimer = nullptr;

	// ���ߎ���(����FixedUpdate�܂ł̕␳�̂���)
	float excess = 0.0f;


#ifdef DEBUG
	// �e�X�g�̂��߂Ɉ�U������Œ��ڕێ�
	// TODO
	// �ŏI�I�ɂ�DebugManager�ɂ܂Ƃ߂�
	Profiler profiler;

	int num = 0;
	int numView = 0;
	int fixedNum = 0;
	int fixedNumView = 0;
	std::unique_ptr<Timer<SceneManager>> debugTimer = nullptr;
#endif
};