#pragma once
#include <memory>

class SceneBase;

// �v���C���[�̃C�x���g���Q�[���V�[���ɓ`���邽�߂̃C���^�[�t�F�[�X
class PlayerEventNotifier
{
public:
	PlayerEventNotifier(std::shared_ptr<SceneBase> game_scene_);

public:
	void SuccessJustAvoid();

private:
	std::weak_ptr<SceneBase> gameScene;
};