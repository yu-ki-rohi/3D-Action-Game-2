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

#pragma region 非公開関数
private:

#pragma endregion


#pragma region メンバ変数
private:
	std::weak_ptr<CollisionManager> collisionManager;
#pragma endregion
};