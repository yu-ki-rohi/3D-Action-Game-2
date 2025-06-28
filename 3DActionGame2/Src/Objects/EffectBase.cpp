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

	// updateTimerが存在するなら時間をリサイズする
	ResizeTimer(past_local_time_scale);
	
}

void EffectBase::MultiplyLocalTimeScaleBy(float multiplier_)
{
	float past_local_time_scale = localTimeScale;
	localTimeScale *= multiplier_;

	// 誤差修正
	const float error = 0.001f;
	if (fabsf(1.0f - localTimeScale) < error)
	{
		localTimeScale = 1.0f;
	}

	// updateTimerが存在するなら時間をリサイズする
	ResizeTimer(past_local_time_scale);
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
	return TimerFactory::CreateTimer(time_ * time_magni, shared_from_this(), this, &EffectBase::ToTheNextImage);
}

void EffectBase::ToTheNextImage()
{
	index++;
}

void EffectBase::ResizeTimer(float past_local_time_scale_)
{
	if (localTimeScale == 0.0f) { return; }
	if (!updateTimer) { return; }

	float remaining_time = updateTimer->GetRemainingTime();
	remaining_time *= past_local_time_scale_;

	float time_magni = 1.0f;
	time_magni /= localTimeScale;
	updateTimer->ResizeTime(remaining_time * time_magni);
}