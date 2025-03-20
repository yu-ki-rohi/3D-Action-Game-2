#pragma once
#include <DxLib.h>
#include <memory>

class SceneManager;

class App
{
public:

#pragma region �R���X�g���N�^�[�E�f�X�g���N�^�[
	App();
	~App();
#pragma endregion
	
#pragma region �A�N�Z�T

#pragma endregion
	
#pragma region ���J�֐�
	void Run();
#pragma endregion

#pragma region ����J�֐�
private:
	bool InitApp();
	void TermApp();

	void MainLoop();

#pragma endregion

#pragma region ����J�����o�ϐ�
private:
	LONGLONG systemTime = GetNowHiPerformanceCount();

	std::unique_ptr<SceneManager> sceneManager;


#pragma endregion

};