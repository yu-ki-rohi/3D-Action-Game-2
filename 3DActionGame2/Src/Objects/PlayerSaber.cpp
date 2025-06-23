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
	attackCollider(Vector3(53.0f, 40.0f, 5.0f), Vector3(135.0f, 18.0f, 12.0f), Vector3(10.0f, -8.0f, 30.0f)),
	bodyCollider(Vector3(0.0f, -5.0f, -15.0f), Vector3(45.0f, 150.0f, 45.0f), Vector3(0.0f, 0.0f, 0.0f))
{

}

void PlayerSaber::Start()
{
	CharacterBase::Start();

	if (!IsActive()) { return; }
	attackCollider.SetIsEnabled(false);
	attackCollider.SetOwner(shared_from_this());
	bodyCollider.SetOwner(shared_from_this());

	bodyCollider.AddObserver(characterStatus);

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
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<PlayerSaber>>(this, &PlayerSaber::IgnitAttackAnimation));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		XINPUT_BUTTON_B,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<PlayerSaber>>(this, &PlayerSaber::IgnitRollAnimation));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		InputManager::Stick::Left,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<PlayerSaber>>(this, &PlayerSaber::IgnitWalkAnimation));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		InputManager::Stick::Left,
		InputManager::State::Release,
		std::make_shared<MemberFunctionPointerContainer<PlayerSaber>>(this, &PlayerSaber::IgnitIdleAnimation));
}

void PlayerSaber::FixedUpdate()
{

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

void PlayerSaber::IgnitIdleAnimation()
{
	if (!canMove) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::Idle, 0.0f, 4.6f, true);
}

void PlayerSaber::IgnitWalkAnimation()
{
	if (!canMove) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::WalkF, 0.0f, 4.6f, true);
}

void PlayerSaber::IgnitAttackAnimation()
{
	if (!animator) { return; }
	animator->SetNextAnim(AKind::Attack00, 0.0f, 4.6f);
	canMove = false;
	TimerFactory::CreateTimer(1.0f, this, &PlayerSaber::FinishAttack);
}

void PlayerSaber::IgnitRollAnimation()
{
	if (!canMove) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::Avoid, 0.0f, 4.6f);
	characterStatus->SetIsInvincible(true);
	//canMove = false;
	TimerFactory::CreateTimer(1.0f, this, &PlayerSaber::FinishAttack);
}

void PlayerSaber::FinishAttack()
{
	canMove = true;
	characterStatus->SetIsInvincible(false);
	float x, y;
	so->GetFloatx2(x, y);
	if (x != 0.0f && y != 0.0f)
	{
		animator->SetNextAnim(AKind::WalkF, Animator::Immediately, 4.6f, true);
	}
	else
	{
		animator->SetNextAnim(AKind::Idle, Animator::Immediately, 4.6f, true);
	}
}