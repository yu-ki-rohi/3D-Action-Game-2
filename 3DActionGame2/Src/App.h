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
	// ����������
	bool InitApp();
	// �I������
	void TermApp();

	// ���C�����[�v
	void MainLoop();

private:
	// �V�X�e������
	// �o�ߎ��Ԏ擾�p
	LONGLONG systemTime = GetNowHiPerformanceCount();
	std::unique_ptr<SceneManager> sceneManager;


};