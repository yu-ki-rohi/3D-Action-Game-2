#pragma once
#include <memory>
class CollisionManager;
class Collider;
class ObjectBase;

// コライダーの登録・登録解除を行うためのインターフェース
// TODO : c++ public関数のみにして多重継承させる形に変更
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