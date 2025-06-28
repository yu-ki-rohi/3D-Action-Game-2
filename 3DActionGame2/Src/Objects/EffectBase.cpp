#include "EffectBase.h"
#include "Components/Transform.h"
#include "../Assets/EffectResource.h"
#include "../Systems/TimerFactory.h"

EffectBase::EffectBase(std::shared_ptr<EffectResource> effect_resource_, Vector3 position_, Vector3 scale_) :
	effectResource(effect_resource_),
	transform(std::make_shared<Transform>(position_, scale_)),
	index(0),
	localTimeScale(1.0f)
{

}

ObjectBase::Tag EffectBase::GetTag() const
{
	return Tag::Effect;
}

void EffectBase::SetLocalTimeScale(float time_scale_)
{
	float past_local_time_scale = localTimeScale;
	localTimeScale = time_scale_;

	// updateTimer‚ª‘¶Ý‚·‚é‚È‚çŽžŠÔ‚ðƒŠƒTƒCƒY‚·‚é
	if (!updateTimer) { return; }

	float remaining_time = updateTimer->GetRemainingTime();
	remaining_time *= past_local_time_scale;

	float time_magni = 1.0f;
	if (localTimeScale != 0.0f)
	{
		time_magni /= localTimeScale;
	}

	updateTimer->ResizeTime(remaining_time * time_magni);
}

void EffectBase::Start()
{
	if (transform == nullptr)
	{
		isActive = false;
		return;
	}
}

void EffectBase::Render()
{
	DrawBillboard3D(transform->Position.ToVECTOR(), 0.5f, 0.5f, transform->Scale.x, transform->Scale.y, effectResource->Handles[index], true);
}


char EffectBase::GetIndex()
{
	return index;
}


std::shared_ptr<TimerBase> EffectBase::CreateUpdateTimer(float time_)
{
	float time_magni = 1.0f;
	if (localTimeScale != 0.0f)
	{
		time_magni /= localTimeScale;
	}
	return TimerFactory::CreateTimer(time_ * time_magni, this, &EffectBase::ToTheNextImage);
}

void EffectBase::ToTheNextImage()
{
	index++;
}