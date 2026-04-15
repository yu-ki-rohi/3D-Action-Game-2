#include "Animator.h"
#include "DxLib.h"
#include "../../Assets/AnimationResource.h"
#include "../../Systems/Time.h"

Animator::Animator(std::shared_ptr<AnimationResource> anim_resource_) :
	animResource(anim_resource_)
{
	if (animResource == nullptr) { return; }
	SetNextAnim(AKind::Idle);
}

float Animator::GetAnimationProgressPercentage()
{
	if (animResource == nullptr) { return 0.0f; }
	float duration = animResource->AnimationParameters[currentAnim.Kind]->Duration;
	if (duration == 0) { return 1.0f; }
	return currentAnim.PlayTime / duration;
}

float Animator::GetAnimationTimeByNormalizedValue(float normalized_value_)
{
	if (animResource == nullptr) { return 0.0f; }
	return animResource->AnimationParameters[currentAnim.Kind]->Duration * normalized_value_;
}

float Animator::GetTimeUntilStartTransition()
{
	if (animResource == nullptr) { return 0.0f; }
	if (isTransitioningImmediately)
	{
		return animResource->AnimationParameters[nextAnim.Kind]->TransitionOutStartTime - nextAnim.PlayTime;
	}
	else
	{
		return animResource->AnimationParameters[currentAnim.Kind]->TransitionOutStartTime - currentAnim.PlayTime;
	}
}

bool Animator::IsTransitioning()
{
	return isTransitioningImmediately || previousAnim.Handle != -1;
}


void Animator::SetNextAnim(AKind anim_kind_, AnimTransitionType transition_type_)
{
	if (animResource == nullptr) { return; }
	nextAnim.Kind = anim_kind_;
	nextAnim.Handle = animResource->Handles[anim_kind_];
	nextAnim.AttachIndex = -1;
	nextAnim.PlayTime = 0.0f;

	float duration = animResource->AnimationParameters[anim_kind_]->Duration;
	if (duration != 0)
	{
		nextAnim.DurationReciprocal = 1.0f / duration;
	}
	else
	{
		nextAnim.DurationReciprocal = 0.0f;
	}
	int anim_index = 0;	// ここは現状のデータ的に0で固定
	nextAnim.TotalTime = MV1GetAnimTotalTime(nextAnim.Handle, anim_index);

	if (transition_type_ == AnimTransitionType::Immediately)
	{
		isTransitioningImmediately = true;
	}
}

void Animator::SetAnimTimerAdjuster(float value_)
{
	animTimerAdjuster = value_;
}

void Animator::SetupRenderAnim(int model_handle_)
{
	int anim_index = 0;// 現状のデータ的にこの値は0で固定
	currentAnim.AttachIndex = MV1AttachAnim(model_handle_, anim_index, currentAnim.Handle);
	MV1SetAttachAnimTime(model_handle_, currentAnim.AttachIndex, ConvertPlayTimeToTimeOnData(currentAnim) + animTimerAdjuster);
	if (previousAnim.Handle == -1) { return; }
	previousAnim.AttachIndex = MV1AttachAnim(model_handle_, anim_index, previousAnim.Handle);
	MV1SetAttachAnimTime(model_handle_, previousAnim.AttachIndex, ConvertPlayTimeToTimeOnData(previousAnim) + animTimerAdjuster);
	// アニメーションブレンド
	MV1SetAttachAnimBlendRate(model_handle_, currentAnim.AttachIndex, currentAnimBlendRate);
	MV1SetAttachAnimBlendRate(model_handle_, previousAnim.AttachIndex, 1.0f - currentAnimBlendRate);
}

void Animator::DetachAnim(int model_handle_)
{
	MV1DetachAnim(model_handle_, currentAnim.AttachIndex);
	if (previousAnim.AttachIndex != -1)
	{
		MV1DetachAnim(model_handle_, previousAnim.AttachIndex);
	}
	currentAnim.AttachIndex = -1;
	previousAnim.AttachIndex = -1;
}

void Animator::Update(float elapsed_time_)
{
	if (animResource == nullptr) { return; }

	AdvanceTime(currentAnim, elapsed_time_);

	HandleTransition();

	UpdateBlendRate(elapsed_time_);

}

float Animator::ConvertPlayTimeToTimeOnData(const AnimInstance& anim_instance_)
{
	std::shared_ptr<AnimationParameters> parameters = animResource->AnimationParameters[anim_instance_.Kind];
	
	float start_offset = parameters->StartOffset;
	float end_offset = parameters->EndOffset;

	return start_offset + (anim_instance_.TotalTime - start_offset - end_offset) * anim_instance_.DurationReciprocal * anim_instance_.PlayTime;
}

void Animator::AdvanceTime(AnimInstance& anim_instance_, float elapsed_time_)
{
	anim_instance_.PlayTime += elapsed_time_ * animSpeed;
	HandleLooping(anim_instance_);
}

void Animator::HandleLooping(AnimInstance& anim_instance_)
{
	std::shared_ptr<AnimationParameters> anim_parameters = animResource->AnimationParameters[anim_instance_.Kind];

	if (anim_parameters->IsLoop == false ||
		anim_instance_.PlayTime < anim_parameters->Duration)
	{
		return;
	}
	anim_instance_.PlayTime = 0.0f;
}

void Animator::HandleTransition()
{
	// 次のアニメーションハンドルが設定されていない場合は処理しない
	// 
	if (nextAnim.Handle == -1 ||
		(isTransitioningImmediately == false &&
		 currentAnim.PlayTime < animResource->AnimationParameters[currentAnim.Kind]->TransitionOutStartTime))
	{
		return;
	}
	previousAnim = currentAnim;
	currentAnim = nextAnim;
	nextAnim.Reset();
	isTransitioningImmediately = false;
}

void Animator::UpdateBlendRate(float elapsed_time_)
{
	if (previousAnim.Handle == -1) { return; }
	AdvanceTime(previousAnim, elapsed_time_);

	float out_duration = animResource->AnimationParameters[previousAnim.Kind]->TransitionOutDuration;
	float in_duration = animResource->AnimationParameters[currentAnim.Kind]->TransitionInDuration;
	float min_duration = (out_duration < in_duration) ? out_duration : in_duration;

	if (min_duration == 0.0f)
	{
		min_duration = minChangingTime;
	}

	if (currentAnim.PlayTime < min_duration)
	{
		currentAnimBlendRate = currentAnim.PlayTime / min_duration;
		return;
	}

	currentAnimBlendRate = 1.0f;
	previousAnim.Reset();
}