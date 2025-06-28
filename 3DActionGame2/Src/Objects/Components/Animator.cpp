#include "Animator.h"
#include "DxLib.h"
#include "../../Assets/AnimationResource.h"

Animator::Animator(int idle_anim_handle_) :
	currentAnimHandle(idle_anim_handle_)
{
	animHandles[AnimType::Idle] = idle_anim_handle_;

	int anim_index = 0;
	animTime = MV1GetAnimTotalTime(idle_anim_handle_, anim_index);
}

Animator::Animator(std::shared_ptr<AnimationResource> anim_resource_) :
	animResource(anim_resource_)
{
	if (animResource)
	{
		currentAnimHandle = animResource->Handles[AKind::Idle];
		int anim_index = 0;
		animTime = MV1GetAnimTotalTime(currentAnimHandle, anim_index);
	}
}

float Animator::GetAnimationProgressPercentage()
{
	if (animTime == 0) { return 1.0f; }
	return currentAnimTimer / animTime;
}

float Animator::GetAnimationTimeByNormalizedValue(float normalized_value_)
{
	return animTime * normalized_value_;
}

void Animator::RegisterAnim(AnimType anim_type_, int anim_handle_)
{
	animHandles[anim_type_] = anim_handle_;
}

void Animator::SetNextAnim(AnimType anim_type_, bool is_loop_)
{
	nextAnimHandle = animHandles[anim_type_];
	nextIsLoop = is_loop_;
}

void Animator::SetNextAnim(AnimType anim_type_, float start_changing_time_, float changing_time_, bool is_loop_)
{
	// changingTimeに0や負数が代入されることを防ぐ
	if (changing_time_ < minChangingTime) { changing_time_ = minChangingTime; }

	nextAnimHandle = animHandles[anim_type_];
	startChangingTime = start_changing_time_;
	changingTime = changing_time_;
	nextIsLoop = is_loop_;
}

void Animator::SetNextAnim(AKind anim_kind_, float start_changing_time_, float changing_time_, bool is_loop_)
{
	// AnimType版とほぼ共通なので、共通部分を関数化するか、そもそもあちらはSample用なのでいずれ消すか

	// nullチェック
	if (!animResource) { return; }

	// changingTimeに0や負数が代入されることを防ぐ
	if (changing_time_ < minChangingTime) { changing_time_ = minChangingTime; }

	nextAnimHandle = animResource->Handles[anim_kind_];
	nextAnimSpeed = animResource->AnimSpeed[anim_kind_];
	startChangingTime = start_changing_time_;
	changingTime = changing_time_;
	nextIsLoop = is_loop_;
}

void Animator::SetAnimTimerAdjuster(float value_)
{
	animTimerAdjuster = value_;
}

void Animator::SetupRenderAnim(int model_handle_)
{
	int anim_index = 0;

	// アニメーションのアタッチ
	currentAttachIndex = MV1AttachAnim(model_handle_, anim_index, currentAnimHandle);
	MV1SetAttachAnimTime(model_handle_, currentAttachIndex, currentAnimTimer + animTimerAdjuster);
	
	// アニメーション遷移しない場合はここで終了
	if (currentAnimTimer < startChangingTime ||
		nextAnimHandle == -1)
	{
		nextAttachIndex = -1;
		return;
	}

	// 遷移先アニメーションのアタッチ
	nextAttachIndex = MV1AttachAnim(model_handle_, anim_index, nextAnimHandle);
	MV1SetAttachAnimTime(model_handle_, nextAttachIndex, nextAnimTimer + animTimerAdjuster);

	float nextRate = nextAnimTimer / changingTime;

	// アニメーションブレンド
	MV1SetAttachAnimBlendRate(model_handle_, nextAttachIndex, nextRate);
	MV1SetAttachAnimBlendRate(model_handle_, currentAttachIndex, 1.0f - nextRate);

}

void Animator::DetachAnim(int model_handle_)
{
	MV1DetachAnim(model_handle_, currentAttachIndex);
	if (nextAttachIndex != -1)
	{
		MV1DetachAnim(model_handle_, nextAttachIndex);
	}
}

void Animator::Update(float elapsed_time_)
{
	// アニメーション遷移が始まった瞬間かどうかを判定
	float evaluation = currentAnimTimer - startChangingTime;
	evaluation *= evaluation + elapsed_time_ * currentAnimSpeed;
	isStartChangeAnimNow = evaluation > 0 ? false : true;

	currentAnimTimer += elapsed_time_ * currentAnimSpeed;

	// アニメーションループ処理
	if (currentAnimTimer >= animTime &&
		isLoop)
	{
		currentAnimTimer = 0.0f;
	}

	isFinishChangeAnimNow = false;
	// アニメーション遷移しない場合はここで終了
	if (currentAnimTimer < startChangingTime ||
		nextAnimHandle == -1) return;

	nextAnimTimer += elapsed_time_ * nextAnimSpeed;

	if (nextAnimTimer / changingTime < 1.0f) return;
	currentAnimHandle = nextAnimHandle;
	currentAnimTimer = nextAnimTimer;
	currentAnimSpeed = nextAnimSpeed;

	int anim_index = 0;
	animTime = MV1GetAnimTotalTime(currentAnimHandle, anim_index);

	nextAnimHandle = -1;
	nextAnimTimer = 0.0f;
	isLoop = nextIsLoop;

	isFinishChangeAnimNow = true;
}