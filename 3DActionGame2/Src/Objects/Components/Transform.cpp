#include "Transform.h"
#include <math.h>
#include <DxLib.h>

Vector3 Transform::GetRotation()
{
	return angleRotation;
}

Vector3 Transform::GetForward()
{
	return quartanion.GetForward();
}

Vector3 Transform::GetUp()
{
	return quartanion.GetUp();
}

Vector3 Transform::GetRight()
{
	return quartanion.GetRight();
}


void Transform::RotateAxisX(float angle_)
{
	if (angle_ == 0.0f) return;

	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::RIGHT);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	angleRotation = quartanion.ToEuler();
}

void Transform::RotateAxisY(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::UP);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	angleRotation = quartanion.ToEuler();
}

void Transform::RotateAxisZ(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::FORWARD);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	angleRotation = quartanion.ToEuler();
}

void Transform::RotateAxisRight(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, GetRight());
	new_quartanion *= quartanion;
	quartanion = new_quartanion;

	angleRotation = quartanion.ToEuler();
}

void Transform::Translate(Vector3 vec_)
{
	Position += quartanion.GetForward() * vec_.z;
	Position += quartanion.GetUp() * vec_.y;
	Position += quartanion.GetRight() * vec_.x;
}

Transform::Transform(Vector3 position_) :
	Position(position_)
{

}

Transform::Transform(Vector3 position_, Vector3 scale_) :
	Position(position_),
	Scale(scale_)
{

}

#ifdef DEBUG
void Transform::DebugDrow()
{
	VECTOR position = Position.ToVECTOR();
	float length = 20.0f;
	DrawLine3D(position, (Position + GetRight() * length).ToVECTOR(), GetColor(255, 0, 0));
	DrawLine3D(position, (Position + GetUp() * length).ToVECTOR(), GetColor(0, 255, 0));
	DrawLine3D(position, (Position + GetForward() * length).ToVECTOR(), GetColor(0, 0, 255));
}

#endif