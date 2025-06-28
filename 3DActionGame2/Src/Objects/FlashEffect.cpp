#include "FlashEffect.h"
#include "Components/Transform.h"
#include "../Assets/EffectResource.h"
#include "../Systems/TimerBase.h"

FlashEffect::FlashEffect(std::shared_ptr<EffectResource> effect_resource_, Vector3 position_, Vector3 scale_) :
	EffectBase(effect_resource_, position_, scale_)
{

}

void FlashEffect::FixedUpdate()
{
	
}

void FlashEffect::Update(float elapsed_time)
{
	if (GetIndex() >= effectResource->ImageNum)
	{
		isActive = false;
		return;
	}

	if (!updateTimer || !updateTimer->IsActive())
	{
		// ˆê’UŽžŠÔ‚ÍŒÅ’è’l‚Å
		updateTimer = CreateUpdateTimer(0.1f);
	}
}