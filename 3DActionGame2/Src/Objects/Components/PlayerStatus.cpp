#include "PlayerStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"
#include "../../Scenes/PlayerEventNotifier.h"

PlayerStatus::PlayerStatus(std::shared_ptr<PlayerEventNotifier> notifier_) :
	notifier(notifier_)
{

}

void PlayerStatus::Damage(int attack_)
{
	if (isInvincible)
	{
		auto event_notifier = notifier.lock();
		if (!event_notifier) { return; }
		event_notifier->SuccessJustAvoid();
		return;
	}
	CharacterStatus::Damage(attack_);
}


void PlayerStatus::OnTriggerEnter(Collider* other_)
{
	// owner‚ÌŽæ“¾‚ð‚É2“x‚â‚Á‚Ä‚¢‚é‚Ì‚Í­‚µ‹C‚É‚È‚é‚Ì‚ÅAŒã‚ÅC³‚µ‚½‚¢
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_tag = owner->GetTag();

	if (other_tag == ObjectBase::Tag::Enemy)
	{
		CharacterStatus::OnTriggerEnter(other_);
	}
}