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

int CharacterStatus::Damage(int attack_)
{
	if (isInvincible) { return 0; }
	return attack_;
}

void CharacterStatus::OnTriggerEnter(Collider* other_)
{
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_status = owner->GetComponent<CharacterStatus>();
	if (!other_status) { return; }

	// 実行に関してのみなので、一旦引数は適当に
	other_status->Damage(0);

}