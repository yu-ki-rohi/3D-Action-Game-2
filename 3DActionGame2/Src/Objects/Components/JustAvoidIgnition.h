#pragma once
#include "ComponentBase.h"
#include "../ObjectBase.h"

// ������������̃W���X�g����𔭐�������R���|�[�l���g
class JustAvoidIgnition : public ComponentBase
{
public:
	JustAvoidIgnition(ObjectBase::Tag ignit_target_);

	void OnTriggerEnter(Collider* other_) override;

private:
	ObjectBase::Tag ignitTarget;
};