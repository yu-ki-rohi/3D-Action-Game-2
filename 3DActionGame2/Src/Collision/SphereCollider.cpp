#include "SphereCollider.h"
#include "../Mathmatics/Quartanion.h"

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

float SphereCollider::GetRadius() const
{
	return radius;
}

Collider::Type SphereCollider::GetType() const
{
	return Type::Sphere;
}

void SphereCollider::UpdateFromParentMat(const MATRIX& parent_mat_)
{

}