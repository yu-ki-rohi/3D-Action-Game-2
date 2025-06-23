#include "PlayerEventNotifier.h"
#include "SceneBase.h"

PlayerEventNotifier::PlayerEventNotifier(std::shared_ptr<SceneBase> game_scene_) :
	gameScene(game_scene_)
{

}

void PlayerEventNotifier::SuccessJustAvoid()
{
	auto game_scene = gameScene.lock();
	if (!game_scene) { return; }

	game_scene->SuccessJustAvoid();
}