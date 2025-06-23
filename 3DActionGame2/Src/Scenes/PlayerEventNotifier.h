#pragma once
#include <memory>

class SceneBase;

// プレイヤーのイベントをゲームシーンに伝えるためのインターフェース
class PlayerEventNotifier
{
public:
	PlayerEventNotifier(std::shared_ptr<SceneBase> game_scene_);

public:
	void SuccessJustAvoid();

private:
	std::weak_ptr<SceneBase> gameScene;
};