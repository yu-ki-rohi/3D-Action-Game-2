#include "Player.h"
#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/CharacterStatus.h"

#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MFPCFactory.h"
#include "../Systems/TimerFactory.h"
#include "../Common.h"
#include "CameraManager.h"

Player::Player(std::shared_ptr<CameraManager> camera_manager_) :
	canMove(true),
	rollingStep(-1),
	cameraManager(camera_manager_)
{
	
}

ObjectBase::Tag Player::GetTag() const
{
	return Tag::Player;
}

void Player::Start()
{
	CharacterBase::Start();

	if (!IsActive()) { return; }

	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Left, InputManager::Map::Player, so);


	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		XINPUT_BUTTON_B,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &Player::IgnitRolling));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		InputManager::Stick::Left,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &Player::IgnitWalkAnimation));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Player,
		InputManager::Stick::Left,
		InputManager::State::Release,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &Player::IgnitIdleAnimation));
}

void Player::HitStop()
{
	MultiplyLocalTimeScaleBy(hitStopTimeScale); 
	TimerFactory::CreateTimer(hitStopTime, shared_from_this(), this, &Player::FinishHitStop);
}

void Player::UpdateBehavior(float elapsed_time_)
{
	if (!canMove) { return; }
	
	float x, y;
	so->GetFloatx2(x, y);
	if (rollingDirection)
	{
		if (rollingStep == 0)
		{
			float speed = 120.0f;
			transform->Position += *rollingDirection * elapsed_time_ * speed;
		}
		else if (rollingStep == 1)
		{
			if (x != 0.0f && y != 0.0f)
			{
				const std::shared_ptr<CameraManager> camera_manager = cameraManager.lock();
				if (camera_manager == nullptr) { return; }

				Vector3 target_pos = camera_manager->GetTargetPosition();
				Vector3 camera_pos = camera_manager->GetCameraPosition();

				Vector3 forward = (target_pos - camera_pos);
				forward.y = 0.0f;
				forward = forward.Normalize();

				Vector3 right = Vector3::Cross(Vector3::UP, forward);

				Vector3 move_vec = forward * y + right * x;
				move_vec = move_vec.Projection(transform->GetRight());

				float magnification = 7.0f;
				*rollingDirection = (*rollingDirection + move_vec * elapsed_time_ * magnification).Normalize();
			}

			float speed = 80.0f;
			transform->Position += *rollingDirection * elapsed_time_ * speed;
			transform->SetForward(-*rollingDirection);
		}
	}
	else
	{
		if (x != 0.0f && y != 0.0f)
		{
			const std::shared_ptr<CameraManager> camera_manager = cameraManager.lock();
			if (camera_manager == nullptr) { return; }

			Vector3 target_pos = camera_manager->GetTargetPosition();
			Vector3 camera_pos = camera_manager->GetCameraPosition();

			Vector3 forward = (target_pos - camera_pos);
			forward.y = 0.0f;
			forward = forward.Normalize();

			Vector3 right = Vector3::Cross(Vector3::UP, forward);

			float speed = 25.0f;
			Vector3 move_vec = forward * y + right * x;
			transform->Position += move_vec * elapsed_time_ * speed;

			transform->SetForward(-move_vec);
		}
	}
}


void Player::IgnitIdleAnimation()
{
	if (!canMove || rollingStep != -1) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::Idle, 0.0f, 4.6f, true);
}

void Player::IgnitWalkAnimation()
{
	if (!canMove || rollingStep != -1) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::WalkF, 0.0f, 4.6f, true);
}

void Player::IgnitRolling()
{
	if (!canMove || rollingStep != -1) { return; }
	if (!animator) { return; }
	animator->SetNextAnim(AKind::Avoid, 0.0f, 4.6f);
	characterStatus->SetIsInvincible(true);
	rollingStep = 0;
	Vector3 dir = transform->GetForward();
	rollingDirection = std::make_shared<Vector3>(dir.x, dir.y, dir.z);
	// ˆê’U”’l‚Í“K“–‚É
	TimerFactory::CreateTimer(0.32f, shared_from_this(), this, &Player::ProceedToNextRollingStep);
	TimerFactory::CreateTimer(0.78f, shared_from_this(), this, &Player::FinishRolling);
}
void Player::ProceedToNextRollingStep()
{
	rollingStep++;
}
void Player::FinishRolling()
{
	canMove = true;
	characterStatus->SetIsInvincible(false);
	rollingStep = -1;
	rollingDirection = nullptr;
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


void Player::FinishHitStop()
{
	MultiplyLocalTimeScaleBy(hitStopTimeScaleInv);
}