#pragma once

class Collider;

class ComponentBase
{
public:
	// 当たり判定・基底では空関数
	virtual void OnTriggerEnter(const Collider* other_) {}
	virtual void OnTriggerStay(const Collider* other_) {}
	virtual void OnTriggerExit(const Collider* other_) {}

	virtual void OnCollisionEnter(const Collider* other_) {}
	virtual void OnCollisionStay(const Collider* other_) {}
	virtual void OnCollisionExit(const Collider* other_) {}

};