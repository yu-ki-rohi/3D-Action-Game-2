#pragma once
#include <unordered_map>
#include <memory>
#include "ComponentBase.h"
#include "../../DataBase/DataKind.h"

struct AnimationResource;

struct AnimInstance
{
	AKind Kind = AKind::Idle;

	int Handle = -1;

	int AttachIndex = -1;

	float PlayTime = 0.0f;

	// 全体再生時間の長さの逆数
	float DurationReciprocal = 1.0f;

	// 元データのアニメーション時間の長さ
	float TotalTime = 1.0f;

	void Reset()
	{
		Kind = AKind::Idle;
		Handle = -1;
		AttachIndex = -1;
		PlayTime = 0.0f;
		DurationReciprocal = 1.0f;
		TotalTime = 1.0f;
	}
};

enum class AnimTransitionType
{
	Immediately,
	Reserve
};

class Animator : public ComponentBase
{
public:
	Animator(std::shared_ptr<AnimationResource> anim_resource_);

public:
	// アニメーションがどこまで進んでいるかを取得
	float GetAnimationProgressPercentage();

	// アニメーションの全体の時間に引数を掛けたものを取得
	float GetAnimationTimeByNormalizedValue(float normalized_value_);

	float GetTimeUntilStartTransition();
	bool IsTransitioning();

	float GetActivationTime();
	float GetDeactivationTime();

	void SetNextAnim(AKind anim_kind_, AnimTransitionType transition_type_ = AnimTransitionType::Immediately);

	void SetAnimTimerAdjuster(float value_);

	// 現在再生中のものと同一のアニメーションへの遷移を許可するか設定する
	void SetIsAllowedToTransitionSameCurrent(bool value_);

public:

	void SetupRenderAnim(int model_handle_);
	void DetachAnim(int model_handle_);

	void Update(float elapsed_time_);

private:
	float ConvertPlayTimeToTimeOnData(const AnimInstance& anim_instance_);

	void AdvanceTime(AnimInstance& anim_instance_, float elapsed_time_);
	void HandleLooping(AnimInstance& anim_instance_);
	void HandleTransition();
	void UpdateBlendRate(float elapsed_time_);

private:
	// リソースへのスマートポインタ
	const std::shared_ptr<AnimationResource> animResource;


	// 新形式
	AnimInstance previousAnim;
	AnimInstance currentAnim;
	AnimInstance nextAnim;

	float animSpeed = 1.0f;

	// アニメーションの再生時間をずらす際に使用
	// 当たり判定の付け方を少し先の再生時間にする、などの用途
	float animTimerAdjuster = 0.0f;

	float currentAnimBlendRate = 1.0f;

	bool isTransitioningImmediately = false;

	bool isAllowedToTransitionSameCurrent = false;

private:
	static constexpr float minChangingTime = 0.001f;

};