#include "PlayerStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"
#include "../../Scenes/PlayerEventNotifier.h"
#include "../ObjectFactory.h"
#include "../../Mathmatics/Vector3.h"
#include "../Player.h"
#include "../../Audio/AudioManager.h"

PlayerStatus::PlayerStatus(std::shared_ptr<Player> owner_, std::shared_ptr<PlayerEventNotifier> notifier_, std::shared_ptr<ObjectFactory> object_factory_) :
	owner(owner_),
	notifier(notifier_),
	objectFactory(object_factory_)
{

}

int  PlayerStatus::Damage(int attack_)
{
	if (isInvincible)
	{
		auto event_notifier = notifier.lock();
		if (!event_notifier) { return 0; }
		event_notifier->SuccessJustAvoid();
		return 0;
	}
	return CharacterStatus::Damage(attack_);
}


void PlayerStatus::OnTriggerEnter(Collider* other_)
{
	auto other_owner = other_->GetOwner();
	if (!other_owner) { return; }

	auto other_tag = other_owner->GetTag();

	if (other_tag == ObjectBase::Tag::Enemy)
	{
		auto other_status = other_owner->GetComponent<CharacterStatus>();
		if (!other_status) { return; }

		// ŽÀs‚ÉŠÖ‚µ‚Ä‚Ì‚Ý‚È‚Ì‚ÅAˆê’Uˆø”‚Í“K“–‚É
		int damage = other_status->Damage(1);
		if (damage > 0)
		{
			auto object_factory = objectFactory.lock();
			if (!object_factory) { return; }
			// ˆø”‚Íˆê’U“K“–‚É
			object_factory->CreateSlashEffect(other_->GetHitPosition(), 30.0f, 0.0f);
			AudioManager::Instance().PlaySoundEffect(SEKind::HitSlash);
		}

		auto player = owner.lock();
		if (!player) { return; }
		player->HitStop();
	}
}