#include "Player.h"
#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Systems/TimerFactory.h"
#include "../Common.h"
#include "CameraManager.h"

Player::Player(std::shared_ptr<CameraManager> camera_manager_) :
	canMove(true),
	cameraManager(camera_manager_)
{
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Left, InputManager::Map::Player, so);
}

ObjectBase::Tag Player::GetTag() const
{
	return Tag::Player;
}

void Player::UpdateBehavior(float elapsed_time_)
{
	if (!canMove) { return; }
	
	float x, y;
	so->GetFloatx2(x, y);
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

		float speed = 30.0f;
		Vector3 move_vec = forward * y + right * x;
		transform->Position += move_vec * elapsed_time_ * speed;

		transform->SetForward(-move_vec);
	}
}