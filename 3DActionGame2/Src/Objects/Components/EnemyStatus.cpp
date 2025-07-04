#include "EnemyStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"


EnemyStatus::EnemyStatus(std::shared_ptr<Enemy> owner_) :
	owner(owner_)
{

}

void EnemyStatus::OnTriggerEnter(Collider* other_)
{
	// ownerの取得をに2度やっているのは少し気になるので、後で修正したい
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_tag = owner->GetTag();

	if (other_tag == ObjectBase::Tag::Player)
	{
		CharacterStatus::OnTriggerEnter(other_);
	}
}