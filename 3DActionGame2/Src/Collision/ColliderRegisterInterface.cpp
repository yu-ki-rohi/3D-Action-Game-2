#include "ColliderRegisterInterface.h"
#include "CollisionManager.h"

ColliderRegisterInterface::ColliderRegisterInterface(std::shared_ptr<CollisionManager> collision_manager_) :
	collisionManager(collision_manager_)
{

}

void ColliderRegisterInterface::RegisterBody(std::shared_ptr<ObjectBase> owner_, Collider* collider_)
{
	std::shared_ptr<CollisionManager> collision_manager = collisionManager.lock();
	if (collision_manager == nullptr) { return; }
	collision_manager->RegisterBody(owner_, collider_);
}

void ColliderRegisterInterface::RegisterTrigger(std::shared_ptr<ObjectBase> owner_, Collider* collider_)
{
	std::shared_ptr<CollisionManager> collision_manager = collisionManager.lock();
	if (collision_manager == nullptr) { return; }
	collision_manager->RegisterTrigger(owner_, collider_);
}

void ColliderRegisterInterface::ReleaseBody(const Collider* collider_)
{
	std::shared_ptr<CollisionManager> collision_manager = collisionManager.lock();
	if (collision_manager == nullptr) { return; }
	collision_manager->ReleaseBody(collider_);
}

void ColliderRegisterInterface::ReleaseTrigger(const Collider* collider_)
{
	std::shared_ptr<CollisionManager> collision_manager = collisionManager.lock();
	if (collision_manager == nullptr) { return; }
	collision_manager->ReleaseTrigger(collider_);
}