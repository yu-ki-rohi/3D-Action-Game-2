#pragma once
#include "EffectBase.h"

class FlashEffect : public EffectBase
{
public:
	FlashEffect(std::shared_ptr<EffectResource> effect_resource_, Vector3 position_, Vector3 scale_);


	void FixedUpdate() override;
	void Update(float elapsed_time) override;
};