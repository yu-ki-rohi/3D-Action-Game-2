#pragma once
#include "CollisionManager.h"

class ColliderRegisterInterface
{
public:
	ColliderRegisterInterface(const CollisionManager* collision_manager);

#pragma region ����J�֐�
private:

#pragma endregion


#pragma region �����o�ϐ�
private:
	CollisionManager* m_collisionManager;
#pragma endregion
};