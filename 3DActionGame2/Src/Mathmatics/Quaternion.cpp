#include "Quaternion.h"
#include "Vector3.h"
#include <math.h>
#include <DxLib.h>

const Quaternion Quaternion::IDENTITY = Quaternion(0, 0, 0, 1);

Quaternion Quaternion::GetRotateQuaternion(float degree_, Vector3 axis_)
{
	if (axis_.sqrLength() != 1.0f)
	{
		axis_ = axis_.Normalize();
	}
	float theta = degree_ * (float)DX_PI / 180.0f;
	float sin = sinf(theta  * 0.5f);
	float cos = cosf(theta  * 0.5f);
	return Quaternion(axis_.x * sin, axis_.y * sin, axis_.z * sin, cos);
}

Quaternion Quaternion::Slearp(const Quaternion& q_00_, Quaternion q_01_, float t_)
{
	// クォータニオンの補間についてChatGPTに確認し、提示されたソースをベースにしています
	// ベクトル的な演算を作っていなかったので、かなり冗長です

	// 内積
	float dot = q_00_.x * q_01_.x + q_00_.y * q_01_.y + q_00_.z * q_01_.z + q_00_.w * q_01_.w;

	if (dot < 0.0f)
	{
		q_01_.x *= -1.0f;
		q_01_.y *= -1.0f;
		q_01_.z *= -1.0f;
		q_01_.w *= -1.0f;

		dot *= -1.0f;
	}

	// sinが0に近い場合は線形補間で代用
	const float DOT_THRESHOLD = 0.9995f;
	if (dot > DOT_THRESHOLD) {
		// イメージ Quaternion result = q_00_ + t_ * (q_01_ - q_00_);
		float x, y, z, w;

		x = q_01_.x - q_00_.x;
		y = q_01_.y - q_00_.y;
		z = q_01_.z - q_00_.z;
		w = q_01_.w - q_00_.w;

		x *= t_;
		y *= t_;
		z *= t_;
		w *= t_;

		x += q_00_.x;
		y += q_00_.y;
		z += q_00_.z;
		w += q_00_.w;

		
		// イメージ　return Normalize(result);
		float length = x * x + y * y + z * z + w * w;
		length = sqrtf(length);
		if (length == 0) { return Quaternion::IDENTITY; }

		x /= length;
		y /= length;
		z /= length;
		w /= length;

		return Quaternion(x, y, z, w);

	}

	float theta_0 = acos(dot);        // 角度
	float theta = theta_0 * t_;
	float sin_theta = sin(theta);
	float sin_theta_0 = sin(theta_0);

	float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
	float s1 = sin_theta / sin_theta_0;

	float x0, y0, z0, w0, x1, y1, z1, w1;
	x0 = s0 * q_00_.x;
	y0 = s0 * q_00_.y;
	z0 = s0 * q_00_.z;
	w0 = s0 * q_00_.w;

	x1 = s1 * q_01_.x;
	y1 = s1 * q_01_.y;
	z1 = s1 * q_01_.z;
	w1 = s1 * q_01_.w;

	return Quaternion(x0 + x1, y0 + y1, z0 + z1, w0 + w1);
}

Quaternion Quaternion::ConvertFromEular(const Vector3& eular_)
{
	float x, y, z, w;
	x =  cosf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + sinf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	y = -sinf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + cosf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	z =  cosf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + sinf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	w = -sinf(eular_.x  * 0.5f) * sinf(eular_.y  * 0.5f) * sinf(eular_.z  * 0.5f) + cosf(eular_.x  * 0.5f) * cosf(eular_.y  * 0.5f) * cosf(eular_.z  * 0.5f);
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::ConvertFrom3x3Matrix(const Vector3& col_00_, const Vector3& col_01_, const Vector3& col_02_)
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

	return Quaternion(x, y, z, w);
}

Quaternion::Quaternion(float x_, float y_, float z_, float w_) :
	x(x_), y(y_), z(z_), w(w_)
{

}

Vector3 Quaternion::GetForward() const
{
	return -Vector3(
		2 * x * z + 2 * y * w,
		2 * y * z - 2 * x * w,
		2 * w * w + 2 * z * z - 1
	);
}

Vector3 Quaternion::GetUp() const
{
	return Vector3(
		2 * x * y - 2 * z * w,
		2 * w * w + 2 * y * y - 1,
		2 * y * z + 2 * x * w
	);
}

Vector3 Quaternion::GetRight() const
{
	return -Vector3(
		2 * w * w + 2 * x * x - 1,
		2 * x * y + 2 * z * w,
		2 * x * z - 2 * y * w
	);
}

Vector3 Quaternion::ToEuler() const
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
Quaternion Quaternion::operator*(const Quaternion& other)
{
	return Quaternion(
		 w * other.x - z * other.y + y * other.z + x * other.w,
		 z * other.x + w * other.y - x * other.z + y * other.w,
		-y * other.x + x * other.y + w * other.z + z * other.w,
		-x * other.x - y * other.y - z * other.z + w * other.w
	);
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	*this = *this * other;
	return *this;
}