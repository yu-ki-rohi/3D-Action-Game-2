#pragma once
#include <memory>

class Collider;
class ObjectBase;

class IColliderRegister
{
public:
	virtual void RegisterBody(std::shared_ptr<Collider> collider_) = 0;
	virtual void RegisterTrigger(std::shared_ptr<Collider> collider_) = 0;

	virtual void ReleaseBody(const std::shared_ptr<Collider> collider_) = 0;
	virtual void ReleaseTrigger(const std::shared_ptr<Collider> collider_) = 0;
};