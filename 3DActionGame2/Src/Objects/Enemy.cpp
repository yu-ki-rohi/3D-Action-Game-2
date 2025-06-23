#include "Enemy.h"

#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"

#include "../Collision/ColliderRegisterInterface.h"

#include "../Systems/TimerFactory.h"

Enemy::Enemy() :
	attackCollider(Vector3(0.0f, 30.0f, -7.0f), Vector3(30.0f, 100.0f, 30.0f), Vector3(0.0f, -6.0f, 32.0f)),
	bodyCollider(Vector3(0.0f, -10.0f, -5.0f), Vector3(70.0f, 170.0f, 40.0f), Vector3(0.0f, 0.0f, 0.0f))
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
	//attackCollider.SetIsEnabled(false);
	attackCollider.SetOwner(shared_from_this());
	bodyCollider.SetOwner(shared_from_this()); 

	attackCollider.AddObserver(characterStatus);

	UpdateCollider();

	attackCollider.UpdateRadius();
	bodyCollider.UpdateRadius();

	auto collider_register_interface = colliderRegisterInterface.lock();
	if (collider_register_interface)
	{
		collider_register_interface->RegisterBody(shared_from_this(), &bodyCollider);
		collider_register_interface->RegisterTrigger(shared_from_this(), &attackCollider);
	}
	
}

void Enemy::FixedUpdate()
{

}

void Enemy::Render()
{
	CharacterBase::Render();
#ifdef DEBUG
	attackCollider.DebugDrow();
	bodyCollider.DebugDrow();
#endif
}

void Enemy::UpdateBehavior(float elapsed_time_)
{
	if (animator->GetAnimationProgressPercentage() > 0.95f)
	{
		float changing_time = animator->GetAnimationTimeByNormalizedValue(0.05f);
		animator->SetNextAnim(AKind::Attack00, Animator::Immediately, changing_time, false);
	}
}

void Enemy::UpdateCollider()
{
	renderer->SetupModelInfo(*transform, *animator);

	// 武器コライダーの更新
	int frame_index = 11;
	MATRIX frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	attackCollider.UpdateFromParentMat(frame_matrix);

	// 身体コライダーの更新
	frame_index = 1;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	bodyCollider.UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());
}