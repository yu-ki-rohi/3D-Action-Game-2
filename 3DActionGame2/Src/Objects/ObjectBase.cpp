#include "ObjectBase.h"
#include "../Collision/Collider.h"

bool ObjectBase::IsActive()
{
	return isActive;
}

void ObjectBase::OnTriggerEnter(Collider* collider){}
void ObjectBase::OnTriggerStay(Collider* collider){}
void ObjectBase::OnTriggerExit(Collider* collider){}

void ObjectBase::OnCollisionEnter(Collider* collider){}
void ObjectBase::OnCollisionStay(Collider* collider){}
void ObjectBase::OnCollisionExit(Collider* collider){}