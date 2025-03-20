#pragma once
#include "SceneBase.h"
#include <memory>

class ObjectManager;
class ObjectFactory;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();

public:
	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void Initialize() override;
	Type Delete() override;

private:
	std::shared_ptr<ObjectManager> objectManager;
	std::shared_ptr<ObjectFactory> objectFactory;
	Type nextType = Type::Game;
};