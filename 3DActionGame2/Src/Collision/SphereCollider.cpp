#include "SphereCollider.h"
#include "../Mathmatics/Quartanion.h"

SphereCollider::SphereCollider(float radius_)
{
	radius = radius_;
}

SphereCollider::SphereCollider(float radius_, Vector3 position_)
{

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

Quartanion SphereCollider::GetQuartanion() const
{
	return Quartanion::IDENTITY;
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
	int div_num = 32;
	unsigned int white = GetColor(255, 255, 255);
	DrawSphere3D(position.ToVECTOR(), radius, div_num, white, white, FALSE);
}
#endif