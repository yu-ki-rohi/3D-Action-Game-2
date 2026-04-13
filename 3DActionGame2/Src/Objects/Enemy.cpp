#include "Enemy.h"

#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"
#include "Components/JustAvoidIgnition.h"

#include "../AI/EnemyBrain.h"

#include "../Systems/TimerFactory.h"

using namespace EnemyAI;

Enemy::Enemy(int id_, std::shared_ptr<IEnemyDirectiveReader> directive_) :
	attackCollider(std::make_shared<BoxCollider>(Vector3(0.0f, 30.0f, -7.0f), Vector3(30.0f, 100.0f, 30.0f), Vector3(0.0f, -6.0f, 32.0f))),
	bodyCollider(std::make_shared<BoxCollider>(Vector3(0.0f, -10.0f, -5.0f), Vector3(70.0f, 170.0f, 40.0f), Vector3(0.0f, 0.0f, 0.0f))),
	justAvoidIgnitionCollider(std::make_shared<BoxCollider>(Vector3(0.0f, 30.0f, -7.0f), Vector3(70.0f, 140.0f, 80.0f), Vector3(0.0f, -6.0f, 32.0f))),
	isChanging(false),
	brain(std::make_unique<EnemyBrain>(id_, directive_, attackCollider, justAvoidIgnitionCollider))
{

}

ObjectBase::Tag Enemy::GetTag() const
{
	return Tag::Enemy;
}

void Enemy::SetLocalTimeScale(float time_scale_)
{
	CharacterBase::SetLocalTimeScale(time_scale_);
	if (!enableColliderTimer || !disableColliderTimer) { return; }
	enableColliderTimer->SetLocalTimeScale(localTimeScale);
	disableColliderTimer->SetLocalTimeScale(localTimeScale);
}

void Enemy::MultiplyLocalTimeScaleBy(float multiplier_)
{
	CharacterBase::MultiplyLocalTimeScaleBy(multiplier_);
	if (!enableColliderTimer || !disableColliderTimer) { return; }
	enableColliderTimer->SetLocalTimeScale(localTimeScale);
	disableColliderTimer->SetLocalTimeScale(localTimeScale);
}

void Enemy::Start()
{
	CharacterBase::Start();
	if (!IsActive()) { return; }
	justAvoidIgnition = std::make_shared<JustAvoidIgnition>(Tag::Player);

	SetupBrain();

	SetupColliders();
	
}

void Enemy::FixedUpdate()
{
	CharacterBase::FixedUpdate();
}

void Enemy::Render()
{
	CharacterBase::Render();
#ifdef DEBUG
	attackCollider->DebugDrow();
	bodyCollider->DebugDrow();
	justAvoidIgnitionCollider->DebugDrow();

	brain->DebugDrow();
#endif
}

void Enemy::SetupBrain()
{
	brain->SetReference(shared_from_this(), animator, transform);
}

void Enemy::SetupColliders()
{
	attackCollider->SetIsEnabled(false);
	justAvoidIgnitionCollider->SetIsEnabled(false);

	attackCollider->SetOwner(shared_from_this());
	bodyCollider->SetOwner(shared_from_this());
	justAvoidIgnitionCollider->SetOwner(shared_from_this());

	attackCollider->AddObserver(characterStatus);
	justAvoidIgnitionCollider->AddObserver(justAvoidIgnition);

	UpdateCollider();

	attackCollider->UpdateRadius();
	bodyCollider->UpdateRadius();
	justAvoidIgnitionCollider->UpdateRadius();

	auto collider_register_interface = colliderRegisterInterface.lock();
	if (collider_register_interface)
	{
		collider_register_interface->RegisterBody(bodyCollider);
		collider_register_interface->RegisterTrigger(attackCollider);
		collider_register_interface->RegisterTrigger(justAvoidIgnitionCollider);
	}
}

void Enemy::UpdateBehavior(float elapsed_time_)
{
#ifdef TMP_EVAUATION
	float border_percentage = 0.9f;
	if (animator->GetAnimationProgressPercentage() > border_percentage)
	{
		if (isChanging) { return; }
		const char pattern_num = 3;
		int judge = GetRand(pattern_num - 1);
		float changing_time = animator->GetAnimationTimeByNormalizedValue(1.0f - border_percentage);
		float enable_time = 0.85f;
		float disable_time = 1.3f;
		switch (judge)
		{
		case 0:
			animator->SetNextAnim(AKind::Attack00, Animator::Immediately, changing_time, false);
			enableColliderTimer = TimerFactory::CreateTimer(enable_time, shared_from_this(), this, &Enemy::EnableAttackCollider);
			disableColliderTimer = TimerFactory::CreateTimer(disable_time, shared_from_this(), this, &Enemy::DisableAttackCollider);
			break;
		case 1:
			disable_time = 1.45f;
			animator->SetNextAnim(AKind::Attack01, Animator::Immediately, changing_time, false);
			enableColliderTimer = TimerFactory::CreateTimer(enable_time, shared_from_this(), this, &Enemy::EnableAttackCollider);
			disableColliderTimer = TimerFactory::CreateTimer(disable_time, shared_from_this(), this, &Enemy::DisableAttackCollider);
			break;
		case 2:
			animator->SetNextAnim(AKind::Attack02, Animator::Immediately, changing_time, false);
			enableColliderTimer = TimerFactory::CreateTimer(enable_time, shared_from_this(), this, &Enemy::EnableAttackCollider);
			disableColliderTimer = TimerFactory::CreateTimer(disable_time, shared_from_this(), this, &Enemy::DisableAttackCollider);
			break;
		}
		isChanging = true;
	}
	else
	{
		isChanging = false;
	}
#else
	brain->Execute(elapsed_time_);

#endif
}

void Enemy::UpdateCollider()
{
	// アニメーション情報をセット
	renderer->SetupModelInfo(*transform, *animator);

	// 攻撃コライダーの更新
	int frame_index = 11;
	MATRIX frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	attackCollider->UpdateFromParentMat(frame_matrix);

	// 身体コライダーの更新
	frame_index = 1;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	bodyCollider->UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());

	// アニメーション情報を調整
	float anim_timer_adjust = 1.2f;
	animator->SetAnimTimerAdjuster(anim_timer_adjust);
	renderer->SetupModelInfo(*transform, *animator);
	animator->SetAnimTimerAdjuster(0.0f);
	
	// ジャスト回避コライダーの更新
	frame_index = 11;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	justAvoidIgnitionCollider->UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());

}

void Enemy::EnableAttackCollider()
{
	attackCollider->SetIsEnabled(true);
	justAvoidIgnitionCollider->SetIsEnabled(true);
}

void Enemy::DisableAttackCollider()
{
	attackCollider->SetIsEnabled(false);
	justAvoidIgnitionCollider->SetIsEnabled(false);
}