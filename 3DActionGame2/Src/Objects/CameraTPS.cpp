#include "CameraTPS.h"
#include "Components/Transform.h"
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"

CameraTPS::CameraTPS() :
	isReverseVertical(InputManager::Instance().IsReverseVertical()),
	isReverseHorizontal(InputManager::Instance().IsReverseHorizontal())
{
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Right, InputManager::Map::Player, so);
}

CameraBase::Type CameraTPS::GetType() const
{
	return Type::TPS;
}

Vector3 CameraTPS::GetCameraPosition() const
{
	return GetTargetPosition() + transform->GetForward() * 30.0f;
}

Vector3 CameraTPS::GetTargetPosition() const
{
	return target->Position + transform->Position;
}

Vector3 CameraTPS::GetNearShadowAreaPos() const
{
	if (target == nullptr || transform == nullptr) { return Vector3::ZERO; }

	float distance = 30.0f;
	Vector3 forward = -transform->GetForward();
	forward.y = 0.0f;
	Vector3 pos = forward.Normalize() * distance * 2.5f + transform->Position;
	pos += target->Position;
	return pos;
}

Vector3 CameraTPS::GetForward() const
{
	if (transform == nullptr) { return Vector3::FORWARD; }
	return -transform->GetForward();
}

Vector3 CameraTPS::GetRight() const
{
	if (transform == nullptr) { return Vector3::RIGHT; }
	return -transform->GetRight();
}

void CameraTPS::Start()
{

}

void CameraTPS::FixedUpdate()
{

}

void CameraTPS::Update(float elapsed_time_)
{
	float x, y;
	so->GetFloatx2(x, y);
	if (isReverseVertical) { y *= -1.0f; }
	if (isReverseHorizontal) { x *= -1.0f; }
	float horizontal_speed = 120.0f, vertical_speed = 30.0f;
	float border = 0.5f;
	if (abs(x) > border)
	{
		transform->RotateAxisY(x * horizontal_speed * elapsed_time_);
	}

	Vector3 camera_dir = transform->GetForward();
	const float y_max = 0.9f;
	const float y_min = -0.35f;
	// 入力値が規定より大きい　かつ
	// カメラの方向が規定の域を出ていない
	if (abs(y) > border && 
		((y > 0 || camera_dir.y < y_max) &&
		 (y < 0 || camera_dir.y > y_min)))
	{
		transform->RotateAxisRight(y * vertical_speed * elapsed_time_);
	}
}