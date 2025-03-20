#pragma once
#include "ObjectBase.h"
#include <memory>
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Animator.h"

class SimpleObserver;
class SampleCamera;

class SampleCharacter : public ObjectBase
{
public:
	SampleCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_);
public:
	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void RegisterAnim(Animator::AnimType anim_type_, int anim_handle_);

private:
	void Attack();
	void Move();
	void Stop();

	void IncreaseMonochrome();
	void DecreaseMonochrome();


private:
	Transform transform;

	Renderer renderer;
	Animator animator;

	float monochromeRate = 0.0f;

private:


	std::shared_ptr<SimpleObserver> so;
};