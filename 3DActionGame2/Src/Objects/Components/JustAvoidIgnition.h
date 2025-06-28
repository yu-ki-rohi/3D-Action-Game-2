#pragma once
#include "ComponentBase.h"
#include "../ObjectBase.h"

// 当たった相手のジャスト回避を発生させるコンポーネント
class JustAvoidIgnition : public ComponentBase
{
public:
	JustAvoidIgnition(ObjectBase::Tag ignit_target_);

	void OnTriggerEnter(Collider* other_) override;

private:
	ObjectBase::Tag ignitTarget;
};