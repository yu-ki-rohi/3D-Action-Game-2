#include "SphereCollider.h"
#include "../Mathmatics/Quaternion.h"

SphereCollider::SphereCollider(float radius_, Vector3 position_) :
	Collider(radius_)
{
	position = position_;
}

SphereCollider::SphereCollider(float radius_, bool is_collide_once_) :
	Collider(radius_, is_collide_once_)
{
	
}

SphereCollider::SphereCollider(float radius_, Vector3 position_, bool is_collide_once_) :
	Collider(radius_, is_collide_once_)
{
	position = position_;
}

Vector3 SphereCollider::GetPosition() const
{
	return position;
}
Vector3 SphereCollider::GetScale() const
{
	return Vector3(radius, radius, radius);
}
Vector3 SphereCollider::SphereCollider::GetRotate() const
{
	return Vector3::ZERO;
}

Quaternion SphereCollider::GetQuaternion() const
{
	return Quaternion::IDENTITY;
}

Collider::Type SphereCollider::GetType() const
{
	return Type::Sphere;
}

void SphereCollider::UpdateFromParentMat(const MATRIX& parent_mat_)
{
	VECTOR new_pos = VGet(0, 0, 0);
	position = Vector3::ConvertFromVECTOR(VTransform(new_pos, parent_mat_));
}

void SphereCollider::UpdatePosition(const Vector3& new_position_)
{
	position = new_position_;
}

#ifdef DEBUG
void SphereCollider::DebugDrow()
{
	unsigned int color;
	if (IsEnabled())
	{
		color = GetColor(255, 255, 255);
	}
	else
	{
		color = GetColor(185, 185, 185);
	}
	int div_num = 32;
	DrawSphere3D(position.ToVECTOR(), radius, div_num, color, color, FALSE);
}
#endif