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
	float sin = sinf(theta  * 0.5f);
	float cos = cosf(theta  * 0.5f);
	return Quartanion(axis_.x * sin, axis_.y * sin, axis_.z * sin, cos);
}

Quartanion Quartanion::ConvertFromEular(const Vector3& eular_)
{
	float x, y, z, w;
	x =  cosf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + sinf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	y = -sinf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + cosf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	z =  cosf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + sinf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	w = -sinf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + cosf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	return Quartanion(x, y, z, w);
}

Quartanion Quartanion::ConvertFrom3x3Matrix(const Vector3& col_00_, const Vector3& col_01_, const Vector3& col_02_)
{
	float m00, m11, m22;	// 名前短縮のためにローカル変数に入れる
	m00 = col_00_.x;
	m11 = col_01_.y;
	m22 = col_02_.z;

	float trace = m00 + m11 + m22;

	float x, y, z, w;		// クォータニオンの各成分保存用
	float tmp_scaling;		// スケーリングのための一次的な変数
	float tmp_inv;			// 逆数を一次的に保存する用

	// ブロック内の処理はいずれ共通化したいところ
	if (trace >= 0)
	{
		tmp_scaling = 2.0f * sqrtf(trace + 1.0f);
		tmp_inv = 1.0f / tmp_scaling;

		w = 0.25f * tmp_scaling;
		x = (col_01_.z - col_02_.y) * tmp_inv;
		y = (col_02_.x - col_00_.z) * tmp_inv;
		z = (col_00_.y - col_01_.x) * tmp_inv;
	}
	// 以下mXXのどれが一番大きいかで分岐
	else if (m00 > m11 && m00 > m22)
	{
		tmp_scaling = 2.0f * sqrtf(m00 - m11 - m22 + 1.0f);
		tmp_inv = 1.0f / tmp_scaling;

		x = 0.25f * tmp_scaling;
		w = (col_01_.z - col_02_.y) * tmp_inv;
		z = (col_02_.x + col_00_.z) * tmp_inv;
		y = (col_00_.y + col_01_.x) * tmp_inv;
	}
	else if (m11 > m22)
	{
		tmp_scaling = 2.0f * sqrtf(m11 - m00 - m22 + 1.0f);
		tmp_inv = 1.0f / tmp_scaling;

		y = 0.25f * tmp_scaling;
		z = (col_01_.z + col_02_.y) * tmp_inv;
		w = (col_02_.x - col_00_.z) * tmp_inv;
		x = (col_00_.y + col_01_.x) * tmp_inv;
	}
	else
	{
		tmp_scaling = 2.0f * sqrtf(m22 - m00 - m11 + 1.0f);
		tmp_inv = 1.0f / tmp_scaling;

		z = 0.25f * tmp_scaling;
		y = (col_01_.z + col_02_.y) * tmp_inv;
		x = (col_02_.x + col_00_.z) * tmp_inv;
		w = (col_00_.y - col_01_.x) * tmp_inv;
	}

	return Quartanion(x, y, z, w);
}

Quartanion::Quartanion(float x_, float y_, float z_, float w_) :
	x(x_), y(y_), z(z_), w(w_)
{

}

Vector3 Quartanion::GetForward() const
{
	return -Vector3(
		2 * x * z + 2 * y * w,
		2 * y * z - 2 * x * w,
		2 * w * w + 2 * z * z - 1
	);
}

Vector3 Quartanion::GetUp() const
{
	return Vector3(
		2 * x * y - 2 * z * w,
		2 * w * w + 2 * y * y - 1,
		2 * y * z + 2 * x * w
	);
}

Vector3 Quartanion::GetRight() const
{
	return -Vector3(
		2 * w * w + 2 * x * x - 1,
		2 * x * y + 2 * z * w,
		2 * x * z - 2 * y * w
	);
}

Vector3 Quartanion::ToEuler() const
{
	Vector3 forward, up, right;

	forward = GetForward();
	up = GetUp();
	right = GetRight();

	float x, y, z;

	y = asinf(-forward.x);

	// 90°を超えているか判定
	if (Vector3::Dot(forward, Vector3::FORWARD) < 0.0f)
	{
		y = (float)DX_PI - y;
	}
	
	// 0割りケア
	if (cosf(y) != 0)
	{
		x = atanf(-forward.y / forward.z);
		z = atanf(-up.x / right.x);
	}
	else
	{
		x = atanf(up.z / up.y);
		z = 0.0f;
	}

	Vector3 angle = Vector3(x, y, z);
	//angle *= 180.0f / (float)DX_PI;

	return angle;
}

// 回転 * 被回転
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