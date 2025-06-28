#pragma once
#include "ObjectBase.h"

struct Vector3;
struct EffectResource;

class Transform;
class TimerBase;

class EffectBase : public ObjectBase
{
public:
	EffectBase(std::shared_ptr<EffectResource> effect_resource_, Vector3 position_, Vector3 scale_);

	Tag GetTag() const override;

	void SetMonochrome(float rate_) override {}
	void SetLocalTimeScale(float time_scale_) override;
	void MultiplyLocalTimeScaleBy(float multiplier_) override;

	void Start() override;

	void Render() override;
	void RenderShadow() override {}

protected:
	char GetIndex();
	std::shared_ptr<TimerBase> CreateUpdateTimer(float time_);

private:
	void ToTheNextImage();
	void ResizeTimer(float past_local_time_scale_);

protected:
	std::shared_ptr<EffectResource> effectResource;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<TimerBase> updateTimer;


private:
	char index;
	float localTimeScale;
};
