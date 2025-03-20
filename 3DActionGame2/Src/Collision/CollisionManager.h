#pragma once
#include "vector"
#include "ColliderRegisterInterface.h"

// ‘O•ûéŒ¾
class Collider;

class CollisionManager
{
public:
	ColliderRegisterInterface* GetRegisterInterface();

	bool CheckCollision();

private:
	ColliderRegisterInterface registerInterface = ColliderRegisterInterface(this);
};