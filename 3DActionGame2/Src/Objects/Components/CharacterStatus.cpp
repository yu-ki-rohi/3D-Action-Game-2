#include "CharacterStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"

CharacterStatus::CharacterStatus() :
	isInvincible(false)
{

}

void CharacterStatus::SetIsInvincible(bool is_invincible_)
{
	isInvincible = is_invincible_;
}

void CharacterStatus::Damage(int attack)
{
	if (isInvincible) { return; }
}

void CharacterStatus::OnTriggerEnter(Collider* other_)
{
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_status = owner->GetComponent<CharacterStatus>();
	if (!other_status) { return; }

	// ŽÀs‚ÉŠÖ‚µ‚Ä‚Ì‚Ý‚È‚Ì‚ÅAˆê’Uˆø”‚Í“K“–‚É
	other_status->Damage(0);

}