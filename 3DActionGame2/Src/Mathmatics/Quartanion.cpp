#include "Quartanion.h"
#include "Vector3.h"
#include <math.h>
#include <DxLib.h>

const Quartanion Quartanion::IDENTITY = Quartanion(0, 0, 0, 1);

Quartanion Quartanion::GetRotateQuartanion(float degree_, Vector3 axis_)
{
	if (axis_.sqrLength() != 1.0f)
	{
		axis_ = axis_.Normalize();
	}
	float theta = degree_ * (float)DX_PI / 180.0f;
	float sin = sinf(theta / 2.0f);
	float cos = cosf(theta / 2.0f);
	return Quartanion(axis_.x * sin, axis_.y * sin, axis_.z * sin, cos);
}

Quartanion::Quartanion(float x_, float y_, float z_, float w_) :
	x(x_), y(y_), z(z_), w(w_)
{

}

Vector3 Quartanion::GetForward()
{
	return -Vector3(
		2 * x * z + 2 * y * w,
		2 * y * z - 2 * x * w,
		2 * w * w + 2 * z * z - 1
	);
}

Vector3 Quartanion::GetUp()
{
	return Vector3(
		2 * x * y - 2 * z * w,
		2 * w * w + 2 * y * y - 1,
		2 * y * z + 2 * x * w
	);
}

Vector3 Quartanion::GetRight()
{
	return -Vector3(
		2 * w * w + 2 * x * x - 1,
		2 * x * y + 2 * z * w,
		2 * x * z - 2 * y * w
	);
}

Vector3 Quartanion::ToEuler()
{
	Vector3 forward, up, right;

	forward = GetForward();
	up = GetUp();
	right = GetRight();

	float x, y, z;

	y = asinf(right.z);

	if (cosf(y) != 0)
	{
		x = atan(-up.z / forward.z);
		z = atan(-right.y / right.x);
	}
	else
	{
		x = atan(forward.y / up.y);
		z = 0.0f;
	}

	return Vector3(x, y, z);
}

// ‰ñ“] * ”í‰ñ“]
Quartanion Quartanion::operator*(const Quartanion& other)
{
	return Quartanion(
		 w * other.x - z * other.y + y * other.z + x * other.w,
		 z * other.x + w * other.y - x * other.z + y * other.w,
		-y * other.x + x * other.y + w * other.z + z * other.w,
		-x * other.x - y * other.y - z * other.z + w * other.w
	);
}

Quartanion& Quartanion::operator*=(const Quartanion& other)
{
	*this = *this * other;
	return *this;
}