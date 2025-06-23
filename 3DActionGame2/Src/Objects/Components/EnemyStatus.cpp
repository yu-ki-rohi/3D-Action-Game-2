#include "EnemyStatus.h"
#include "../../Collision/Collider.h"
#include "../ObjectBase.h"


void EnemyStatus::OnTriggerEnter(Collider* other_)
{
	// owner�̎擾����2�x����Ă���̂͏����C�ɂȂ�̂ŁA��ŏC��������
	auto owner = other_->GetOwner();
	if (!owner) { return; }

	auto other_tag = owner->GetTag();

	if (other_tag == ObjectBase::Tag::Player)
	{
		CharacterStatus::OnTriggerEnter(other_);
	}
}