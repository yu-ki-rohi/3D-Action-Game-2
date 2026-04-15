#pragma once
#include "ObjectBase.h"
#include <memory>
#include "Components/Transform.h"
#include "Components/Renderer.h"

// テスト表示用
// 不要になれば消す

class SampleStage : public ObjectBase
{
public:
	SampleStage(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_);
public:
	Tag GetTag() const override { return Tag::Stage; }

	void Start() override {}

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;
	void RenderShadow() override;

	void SetMonochrome(float rate_) override { monochromeRate = rate_; }
	void ChangeMonochromeRequest(float initial_rate_, float target_rate_, float duration_) override;
	void SetLocalTimeScale(float time_scale_) override {}
	void MultiplyLocalTimeScaleBy(float multiplier_) override {}

	void IncreaseMonochrome();
	void DecreaseMonochrome();

private:
	void UpdateMonochrome(float elapsed_time_);
private:
	Transform transform;

	Renderer renderer;

	

private:
	float monochromeRate;
	float monochromeInitial;
	float monochromeTarget;
	float monochromeChangeDuration;
	float monochromeChangeTime;
	bool isMonochromeChanging;

private:

};