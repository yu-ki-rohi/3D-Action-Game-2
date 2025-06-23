#pragma once

class Collider;

class ComponentBase
{
public:
	// �����蔻��
	virtual void OnTriggerEnter(Collider* other_) {}
	virtual void OnTriggerStay(Collider* other_){}
	virtual void OnTriggerExit(Collider* other_){}

	virtual void OnCollisionEnter(Collider* other_){}
	virtual void OnCollisionStay(Collider* other_){}
	virtual void OnCollisionExit(Collider* other_){}

};