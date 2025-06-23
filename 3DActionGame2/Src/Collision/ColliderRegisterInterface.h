#pragma once
#include <memory>
class CollisionManager;
class Collider;
class ObjectBase;

class ColliderRegisterInterface
{
public:
	ColliderRegisterInterface(std::shared_ptr<CollisionManager> collision_manager_);

public:
	void RegisterBody(std::shared_ptr<ObjectBase> owner_, Collider* collider_);
	void RegisterTrigger(std::shared_ptr<ObjectBase> owner_, Collider* collider_);

	void ReleaseBody(const Collider* collider_);
	void ReleaseTrigger(const Collider* collider_);

private:
	std::weak_ptr<CollisionManager> collisionManager;
};