#pragma once
#include <memory>
class CollisionManager;

class ColliderRegisterInterface
{
public:
	ColliderRegisterInterface(const CollisionManager* collision_manager);

public:
	void RegisterBody();
	void RegisterAttack();

	void ReleaseBody();
	void ReleaseAttack();

#pragma region ����J�֐�
private:

#pragma endregion


#pragma region �����o�ϐ�
private:
	std::weak_ptr<CollisionManager> collisionManager;
#pragma endregion
};