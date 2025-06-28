#include "JustAvoidIgnition.h"
#include "../../Collision/Collider.h"
#include "CharacterStatus.h"

JustAvoidIgnition::JustAvoidIgnition(ObjectBase::Tag ignit_target_) :
	ignitTarget(ignit_target_)
{

}

void JustAvoidIgnition::OnTriggerEnter(Collider* other_)
{
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_tag = owner->GetTag();

	if (other_tag != ignitTarget) { return; }

	auto other_status = owner->GetComponent<CharacterStatus>();
	if (!other_status) { return; }

	// ジャスト回避を発生させるだけなので、ダメージは与えない
	other_status->Damage(0);
}