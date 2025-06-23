#include "EnemyStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"


void EnemyStatus::OnTriggerEnter(Collider* other_)
{
	// owner‚ÌŽæ“¾‚ð‚É2“x‚â‚Á‚Ä‚¢‚é‚Ì‚Í­‚µ‹C‚É‚È‚é‚Ì‚ÅAŒã‚ÅC³‚µ‚½‚¢
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_tag = owner->GetTag();

	if (other_tag == ObjectBase::Tag::Player)
	{
		CharacterStatus::OnTriggerEnter(other_);
	}
}