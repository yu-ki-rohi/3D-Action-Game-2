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

#pragma region ”ñŒöŠJŠÖ”
private:

#pragma endregion


#pragma region ƒƒ“ƒo•Ï”
private:
	std::weak_ptr<CollisionManager> collisionManager;
#pragma endregion
};