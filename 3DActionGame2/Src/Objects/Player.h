#pragma once
#include "CharacterBase.h"
#include "../Collision/BoxCollider.h"

class CameraManager;
class SimpleObserver;

class Player : public CharacterBase
{
public:
	Player(std::shared_ptr<CameraManager> camera_manager_);
	~Player() = default;

public:
	Tag GetTag() const override;

protected:
	virtual void UpdateBehavior(float elapsed_time_) override;


protected:
	bool canMove;
	std::shared_ptr<SimpleObserver> so;

private:
	std::weak_ptr<CameraManager> cameraManager;

};