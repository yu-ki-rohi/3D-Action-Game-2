#include "Vector3.h"
#include <math.h>

const Vector3 Vector3::FORWARD = Vector3(0, 0, -1);

const Vector3 Vector3::UP = Vector3(0, 1, 0);

const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);

const Vector3 Vector3::ZERO = Vector3(0, 0, 0);

const Vector3 Vector3::ONE = Vector3(1, 1, 1);

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	Vector3 cross = Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);

	return cross;
}

Vector3 Vector3::ConvertFromVECTOR(const VECTOR& vector_)
{
	return Vector3(vector_.x, vector_.y, vector_.z);
}

Vector3 Vector3::ConvertFromXMFLOTA3(const DirectX::XMFLOAT3& xmfloat3_)
{
	return Vector3(xmfloat3_.x, xmfloat3_.y, xmfloat3_.z);
}

Vector3::Vector3() :
	x(0), y(0), z(0)
{

}

Vector3::Vector3(float x_, float y_, float z_) :
	x(x_), y(y_), z(z_)
{

}

Vector3 Vector3::Normalize()
{
	// 0割りはオペレータでケア
	return *this / Length();
}

float Vector3::Length()
{
	return sqrtf(sqrLength());
}

float Vector3::sqrLength()
{
	return Dot(*this, *this);
}

Vector3 Vector3::Projection(Vector3 axis_)
{
	axis_ = axis_.Normalize();
	
	return axis_ * Dot(*this, axis_);
}

Vector3 Vector3::Projection(Vector3 axis_01_, Vector3 axis_02_)
{
	axis_01_ = axis_01_.Normalize();
	axis_02_ = axis_02_.Normalize();

	return *this - Projection(Cross(axis_01_, axis_02_));
}

VECTOR Vector3::ToVECTOR()
{
	VECTOR vector = VGet(x, y, z);
	return vector;
}

DirectX::XMFLOAT3 Vector3::ToXMFLOAT3()
{
	DirectX::XMFLOAT3 xmfloat3 = { x, y ,z };
	return xmfloat3;
}


#pragma region Vector3 operator

Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

bool operator==(const Vector3& a, const Vector3& b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

Vector3 operator+(const Vector3& a, const float& b)
{
	return Vector3(a.x + b, a.y + b, a.z + b);
}

Vector3 operator-(const Vector3& a, const float& b)
{
	return Vector3(a.x - b, a.y - b, a.z - b);
}

Vector3 operator*(const Vector3& a, const float& b)
{
	return Vector3(a.x * b, a.y * b, a.z * b);
}

Vector3 operator/(const Vector3& a, float b)
{
	if (b == 0) b = 1;
	return a * (1.0f / b);
}

Vector3 operator+(const float& a, const Vector3& b)
{
	return Vector3(b.x + a, b.y + a, b.z + a);
}

Vector3 operator-(const float& a, const Vector3& b)
{
	return Vector3(b.x - a, b.y - a, b.z - a);
}

Vector3 operator*(const float& a, const Vector3& b)
{
	return Vector3(b.x * a, b.y * a, b.z * a);
}

Vector3 operator-(const Vector3& a)
{
	return Vector3(-a.x, -a.y, -a.z);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	*this = *this + other;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	*this = *this - other;
	return *this;
}

Vector3& Vector3::operator+=(const float& other)
{
	*this = *this + other;
	return *this;
}

Vector3& Vector3::operator-=(const float& other)
{
	*this = *this - other;
	return *this;
}

Vector3& Vector3::operator*=(const float& other)
{
	*this = *this * other;
	return *this;
}
#pragma endregion


