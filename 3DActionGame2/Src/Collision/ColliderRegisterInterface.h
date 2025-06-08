#pragma once
#include <memory>
class CollisionManager;
class Collider;

class ColliderRegisterInterface
{
public:
	ColliderRegisterInterface(std::shared_ptr<CollisionManager> collision_manager_);

public:
	void RegisterBody(const Collider* collider_);
	void RegisterTrigger(const Collider* collider_);

	void ReleaseBody(const Collider* collider_);
	void ReleaseTrigger(const Collider* collider_);

private:
	std::weak_ptr<CollisionManager> collisionManager;
};