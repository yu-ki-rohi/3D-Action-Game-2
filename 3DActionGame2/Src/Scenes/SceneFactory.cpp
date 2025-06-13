#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SampleScene.h"

std::shared_ptr<SceneBase> SceneFactory::Create(SceneBase::Type type_)
{
	std::shared_ptr<SceneBase> new_scene = nullptr;

	// type_ == Type::Finish ‚Íˆ—‚ğ‚µ‚È‚¢
	// >> nullptr ‚ğ•Ô‹p
	// >> SceneManager::IsRunning() ‚ªfalse‚ğ•Ô‹p
	// >> ƒAƒvƒŠI—¹
	switch (type_)
	{
	case SceneBase::Type::Title:
		new_scene = std::make_shared<TitleScene>();
		break;
	case SceneBase::Type::Game:
		new_scene = std::make_shared<GameScene>();
		break;
	case SceneBase::Type::Sample:
		new_scene = std::make_shared<SampleScene>();
		break;
	}

	return new_scene;
}