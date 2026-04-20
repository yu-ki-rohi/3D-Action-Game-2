#include "CharacterBase.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"
#include "../Collision/Collider.h"

CharacterBase::CharacterBase() :
	localTimeScale(1.0f),
	monochromeRate(0.0f),
	monochromeInitial(0.0f),
	monochromeTarget(0.0f),
	monochromeChangeDuration(0.0f),
	monochromeChangeTime(0.0f),
	isMonochromeChanging(false)
{

}

Vector3 CharacterBase::GetPosition()
{
	return transform->Position;
}

void CharacterBase::Start()
{
	if (transform == nullptr ||
		renderer == nullptr ||
		animator == nullptr)
	{
		isActive = false;
	}

	animator->SetIsAllowedToTransitionSameCurrent(true);
	animator->SetNextAnim(AKind::Idle);
	animator->SetIsAllowedToTransitionSameCurrent(false);
}

void  CharacterBase::FixedUpdate()
{
	UpdateCollider();
}

void CharacterBase::Update(float elapsed_time_)
{
	UpdateBehavior(elapsed_time_ * localTimeScale);

	animator->Update(elapsed_time_ * localTimeScale);

	transform->Update(elapsed_time_ * localTimeScale);

	UpdateMonochrome(elapsed_time_);
}

void CharacterBase::Render()
{
	FLOAT4 monocrome_Rate = { monochromeRate,0.0f,0.0f, 0.0f };
	const int constant_index = 22;
	SetPSConstF(constant_index, monocrome_Rate);

	renderer->IsRenderingShadow = false;
	renderer->Render(*transform, *animator);

#ifdef DEBUG
	transform->DebugDrow();
#endif
}

void CharacterBase::RenderShadow()
{
	renderer->IsRenderingShadow = true;
	renderer->Render(*transform, *animator);
}

void CharacterBase::SetComponent(std::shared_ptr<Transform> transform_)
{
	if (transform_ == nullptr) { return; }
	transform = transform_;
	AddComponent(transform);
}

void CharacterBase::SetComponent(std::shared_ptr<Renderer> renderer_)
{
	if (renderer_ == nullptr) { return; }
	renderer = renderer_;
}

void CharacterBase::SetComponent(std::shared_ptr<Animator> animator_)
{
	if (animator_ == nullptr) { return; }
	animator = animator_;
}

void CharacterBase::SetComponent(std::shared_ptr <CharacterStatus> character_status_)
{
	if (character_status_ == nullptr) { return; }
	characterStatus = character_status_;
	AddComponent(characterStatus);
}

void CharacterBase::SetColliderRegisterInterface(std::shared_ptr<IColliderRegister> collider_register_interface_)
{
	if (collider_register_interface_ == nullptr) { return; }
	colliderRegisterInterface = collider_register_interface_;
}

void CharacterBase::SetMonochrome(float rate_)
{
	monochromeRate = rate_;
}

void CharacterBase::ChangeMonochromeRequest(float initial_rate_, float target_rate_, float duration_)
{
	monochromeInitial = initial_rate_;
	monochromeTarget = target_rate_;
	monochromeChangeDuration = duration_;
	monochromeChangeTime = 0.0f;

	isMonochromeChanging = true;
}

void CharacterBase::SetLocalTimeScale(float time_scale_)
{
	localTimeScale = time_scale_;
}

void  CharacterBase::MultiplyLocalTimeScaleBy(float multiplier_)
{
	localTimeScale *= multiplier_;

	// åÎç∑èCê≥
	const float error = 0.001f;
	if (fabsf(1.0f - localTimeScale) < error)
	{
		localTimeScale = 1.0f;
	}
}

void CharacterBase::UpdateMonochrome(float elapsed_time_)
{

	if (isMonochromeChanging = false) { return; }

	if (monochromeChangeTime < monochromeChangeDuration)
	{
		monochromeChangeTime += elapsed_time_;
		monochromeRate = monochromeInitial + (monochromeTarget - monochromeInitial) / monochromeChangeDuration * monochromeChangeTime;
	}
	else
	{
		monochromeRate = monochromeTarget;
		isMonochromeChanging = false;
	}
}