#include "Enemy.h"

#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"
#include "Components/JustAvoidIgnition.h"

#include "../Collision/ColliderRegisterInterface.h"

#include "../Systems/TimerFactory.h"

Enemy::Enemy() :
	attackCollider(Vector3(0.0f, 30.0f, -7.0f), Vector3(30.0f, 100.0f, 30.0f), Vector3(0.0f, -6.0f, 32.0f)),
	bodyCollider(Vector3(0.0f, -10.0f, -5.0f), Vector3(70.0f, 170.0f, 40.0f), Vector3(0.0f, 0.0f, 0.0f)),
	justAvoidIgnitionCollider(Vector3(0.0f, 30.0f, -7.0f), Vector3(70.0f, 140.0f, 80.0f), Vector3(0.0f, -6.0f, 32.0f)),
	isChanging(false)
{

}

ObjectBase::Tag Enemy::GetTag() const
{
	return Tag::Enemy;
}

void Enemy::Start()
{
	CharacterBase::Start();
	if (!IsActive()) { return; }
	justAvoidIgnition = std::make_shared<JustAvoidIgnition>(Tag::Player);

	//attackCollider.SetIsEnabled(false);
	attackCollider.SetOwner(shared_from_this());
	bodyCollider.SetOwner(shared_from_this()); 
	justAvoidIgnitionCollider.SetOwner(shared_from_this());

	attackCollider.AddObserver(characterStatus);
	justAvoidIgnitionCollider.AddObserver(justAvoidIgnition);

	UpdateCollider();

	attackCollider.UpdateRadius();
	bodyCollider.UpdateRadius();
	justAvoidIgnitionCollider.UpdateRadius();

	auto collider_register_interface = colliderRegisterInterface.lock();
	if (collider_register_interface)
	{
		collider_register_interface->RegisterBody(shared_from_this(), &bodyCollider);
		collider_register_interface->RegisterTrigger(shared_from_this(), &attackCollider);
		collider_register_interface->RegisterTrigger(shared_from_this(), &justAvoidIgnitionCollider);
	}
	
}

void Enemy::FixedUpdate()
{
	CharacterBase::FixedUpdate();
}

void Enemy::Render()
{
	CharacterBase::Render();
#ifdef DEBUG
	attackCollider.DebugDrow();
	bodyCollider.DebugDrow();
	justAvoidIgnitionCollider.DebugDrow();
#endif
}

void Enemy::UpdateBehavior(float elapsed_time_)
{
	if (animator->GetAnimationProgressPercentage() > 0.9f)
	{
		if (isChanging) { return; }
		const char pattern_num = 3;
		int judge = GetRand(pattern_num - 1);
		float changing_time = animator->GetAnimationTimeByNormalizedValue(0.1f);
		switch (judge)
		{
		case 0:
			animator->SetNextAnim(AKind::Attack00, Animator::Immediately, changing_time, false);
			break;
		case 1:
			animator->SetNextAnim(AKind::Attack01, Animator::Immediately, changing_time, false);
			break;
		case 2:
			animator->SetNextAnim(AKind::Attack02, Animator::Immediately, changing_time, false);
			break;
		}
		isChanging = true;
	}
	else
	{
		isChanging = false;
	}
}

void Enemy::UpdateCollider()
{
	// アニメーション情報をセット
	renderer->SetupModelInfo(*transform, *animator);

	// 攻撃コライダーの更新
	int frame_index = 11;
	MATRIX frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	attackCollider.UpdateFromParentMat(frame_matrix);

	// 身体コライダーの更新
	frame_index = 1;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	bodyCollider.UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());

	// アニメーション情報を調整
	float anim_timer_adjust = 1.2f;
	animator->SetAnimTimerAdjuster(anim_timer_adjust);
	renderer->SetupModelInfo(*transform, *animator);
	animator->SetAnimTimerAdjuster(0.0f);
	
	// ジャスト回避コライダーの更新
	frame_index = 11;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	justAvoidIgnitionCollider.UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());

}