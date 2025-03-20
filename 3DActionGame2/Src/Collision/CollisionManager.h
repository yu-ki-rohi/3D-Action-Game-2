#pragma once
#include "vector"
#include "ColliderRegisterInterface.h"

// �O���錾
class Collider;

class CollisionManager
{
public:
	ColliderRegisterInterface* GetRegisterInterface();

	bool CheckCollision();

private:
	ColliderRegisterInterface registerInterface = ColliderRegisterInterface(this);
};