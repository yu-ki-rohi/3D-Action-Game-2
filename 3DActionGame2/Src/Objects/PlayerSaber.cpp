#include "PlayerSaber.h"

#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"

#include "../Collision/ColliderRegisterInterface.h"

#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MemberFunctionPointerContainer.h"

#include "../Systems/TimerFactory.h"


PlayerSaber::PlayerSaber(std::shared_ptr<CameraManager> camera_manager_) :
	Player(camera_manager_),
	attackCollider(Vector3(53.0f, 40.0f, 5.0f), Vector3(145.0f, 18.0f, 12.0f), Vector3(10.0f, -8.0f, 30.0f)),
	bodyCollider(Vector3(0.0f, -5.0f, -15.0f), Vector3(45.0f, 150.0f, 45.0f), Vector3(0.0f, 0.0f, 0.0f)),
	attackStep(0)
{

}

void PlayerSaber::Start()
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

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		XINPUT_BUTTON_A,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<PlayerSaber>>(this, &PlayerSaber::IgnitAttackAnimation));

}

void PlayerSaber::FixedUpdate()
{
	CharacterBase::FixedUpdate();
}

void PlayerSaber::Render()
{
	CharacterBase::Render();
#ifdef DEBUG
	attackCollider.DebugDrow();
	bodyCollider.DebugDrow();
#endif
}

void PlayerSaber::UpdateBehavior(float elapsed_time_)
{
	Player::UpdateBehavior(elapsed_time_);
}

void PlayerSaber::UpdateCollider()
{
	renderer->SetupModelInfo(*transform, *animator);

	// 武器コライダーの更新
	int frame_index = 34;
	MATRIX frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	attackCollider.UpdateFromParentMat(frame_matrix);

	// 身体コライダーの更新
	frame_index = 1;
	frame_matrix = MV1GetFrameLocalWorldMatrix(renderer->GetModelHandle(), frame_index);
	bodyCollider.UpdateFromParentMat(frame_matrix);

	animator->DetachAnim(renderer->GetModelHandle());
}

void PlayerSaber::IgnitAttackAnimation()
{
	if (!canMove || rollingStep != -1) { return; }
	if (!animator) { return; }
	float motion_time = 0.7f;
	switch (attackStep)
	{
	case 0:
		animator->SetNextAnim(AKind::Attack00, 0.0f, 4.6f);
		break;
	case 1:
		animator->SetNextAnim(AKind::Attack01, 0.0f, 4.6f);
		motion_time = 0.8f;
		break;
	case 2:
		animator->SetNextAnim(AKind::Attack02, 0.0f, 4.6f);
		motion_time = 0.8f;
		break;
	case 3:
		animator->SetNextAnim(AKind::Attack03, 0.0f, 4.6f);
		motion_time = 1.1f;
		attackStep = -1;
		break;
	default:
		animator->SetNextAnim(AKind::Attack00, 0.0f, 4.6f);
		break;
	}
	attackStep++;
	canMove = false;
	TimerFactory::CreateTimer(motion_time, this, &PlayerSaber::FinishAttack);
	TimerFactory::CreateTimer(motion_time + 0.2f, this, &PlayerSaber::ResetAttackStep);
}


void PlayerSaber::FinishAttack()
{
	canMove = true;
	float x, y;
	so->GetFloatx2(x, y);
	if (x != 0.0f && y != 0.0f)
	{
		animator->SetNextAnim(AKind::WalkF, Animator::Immediately, 4.6f, true);
	}
	else
	{
		float changing_time_parcentage = 0.95f;
		animator->SetNextAnim(AKind::Idle, animator->GetAnimationTimeByNormalizedValue(changing_time_parcentage), animator->GetAnimationTimeByNormalizedValue(1.0f - changing_time_parcentage), true);
	}
}

void PlayerSaber::ResetAttackStep()
{
	if (!canMove) { return; }
	attackStep = 0;
}