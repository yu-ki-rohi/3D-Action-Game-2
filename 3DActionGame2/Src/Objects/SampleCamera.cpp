#include "SampleCamera.h"
#include <DxLib.h>
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include <math.h>

SampleCamera::SampleCamera(Vector3 position_) :
	transform(Transform(position_))
{
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Right, so);
}

Vector3 SampleCamera::GetNearShadowAreaPos()
{
	Vector3 pos = -transform.GetForward() * distance * 2.5f + transform.Position;
	if (target != nullptr)
	{
		pos += target->Position;
	}
	return pos;
}

Vector3 SampleCamera::GetForward()
{
	return -transform.GetForward();
}

Vector3 SampleCamera::GetRight()
{
	return -transform.GetRight();
}

void SampleCamera::FixedUpdate()
{

}

void SampleCamera::Update(float elapsed_time_)
{
	float x, y;
	so->GetFloatx2(x, y);
	if (abs(x) > 0.5f)
	{
		transform.RotateAxisY(x * horizontalSpeed * elapsed_time_);
	}
	if (abs(y) > 0.4f)
	{
		transform.RotateAxisRight(-y * varticalSpeed * elapsed_time_);
	}

}

void SampleCamera::Render()
{

}

void SampleCamera::RenderShadow()
{

}

void SampleCamera::SetTarget(Transform* target_)
{
	target = target_;
}

void SampleCamera::SetCameraInfo()
{
	if (target == nullptr) return;
	Vector3 up = Vector3::UP;
	SetCameraPositionAndTargetAndUpVec(
		(transform.GetForward() * distance + target->Position + transform.Position).ToVECTOR(),
		(target->Position + transform.Position).ToVECTOR(),
		up.ToVECTOR()
	);
}