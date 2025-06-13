#pragma once
#include "ObjectBase.h"
#include <memory>
#include "Components/Transform.h"
#include "Components/Renderer.h"

class SampleStage : public ObjectBase
{
public:
	SampleStage(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_);
public:
	Tag GetTag() { return Tag::Stage; }

	void Start() override {}

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;
	void RenderShadow() override;

	void SetMonochrome(float rate_) override {}

	void IncreaseMonochrome();
	void DecreaseMonochrome();

private:

private:
	Transform transform;

	Renderer renderer;

	float monochromeRate = 0.0f;

private:

};