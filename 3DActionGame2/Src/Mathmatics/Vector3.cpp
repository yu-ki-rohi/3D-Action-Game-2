#include "Vector3.h"
#include <math.h>
#include "Quartanion.h"

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

Vector3 Vector3::BasisTransformation(const Vector3& vector_, Vector3 axis_01_, Vector3 axis_02_, Vector3 axis_03_, Vector3 origin_)
{
	// 新たな原点からのベクトルに変更
	Vector3 vector = vector_ - origin_;

	// 基底の変換行列を用意
	Vector3* axes[COMPONENTS_NUM] = { &axis_01_, &axis_02_, &axis_03_ };
	float m[COMPONENTS_NUM][COMPONENTS_NUM];	//基底変換行列
	for (int i = 0; i < COMPONENTS_NUM; i++)
	{
		m[0][i] = axes[i]->x;
		m[1][i] = axes[i]->y;
		m[2][i] = axes[i]->z;
	}

	// 逆行列の計算
	// 余因子を使う

	// 行列式の計算
	float determinant =
		  m[0][0] * m[1][1] * m[2][2]
		- m[0][0] * m[1][2] * m[2][1]
		+ m[0][1] * m[1][2] * m[2][0]
		- m[0][1] * m[1][0] * m[2][2]
		+ m[0][2] * m[1][0] * m[2][1]
		- m[0][2] * m[1][1] * m[2][0];

	// 0割り対策
	// 軸が一次独立でない時などは変換不可能
	if (determinant == 0)
	{
		return vector_;
	}

	// 余因子行列生成
	float d[COMPONENTS_NUM][COMPONENTS_NUM];

	// ネスト深すぎ問題
	// opinion
	// 余因子生成部分をラムダ関数にしてしまうとか
	// 全体の長さ的には別関数として切り抜く方がよいかも
	// 但し対処優先度はそんなに高くなさそう
	for (int line = 0; line < COMPONENTS_NUM; line++)
	{
		for (int col = 0; col < COMPONENTS_NUM; col++)
		{
			// 余因子生成
			float excluded_d[COMPONENTS_NUM - 1][COMPONENTS_NUM - 1];
			int tmp_line = 0, tmp_col = 0;
			for (int l = 0; l < COMPONENTS_NUM; l++)
			{
				if (l == line) { continue; }
				for (int c = 0; c < COMPONENTS_NUM; c++)
				{
					if (c == col) { continue; }
					excluded_d[tmp_line][tmp_col] = m[l][c];
					tmp_col++;
				}
				tmp_col = 0;
				tmp_line++;
			}
			
			float sign = 1.0f;	// 符号
			// 繰り返し掛ける代わりに余りを計算して符号決定
			if ((line + col) % 2 != 0)
			{
				sign = -1.0f;
			}
			// 符号 * 行列式
			d[line][col] = sign * (excluded_d[0][0] * excluded_d[1][1] - excluded_d[0][1] * excluded_d[1][0]);
		}
	}

	float e[COMPONENTS_NUM]{};	// elementsの意
	float* vp[COMPONENTS_NUM] = { &vector.x,&vector.y, &vector.z };	// vecotrの要素へのpointerという意味
	float inv_det = 1.0f / determinant;
	// 基底の変換行列の逆行列 * vector の計算を行う
	for (int i = 0; i < COMPONENTS_NUM; i++)
	{
		for (int j = 0; j < COMPONENTS_NUM; j++)
		{
			e[i] += inv_det * d[i][j] * *vp[j];
		}
	}

	return Vector3(e[2], e[1], e[0]);

}


Vector3 Vector3::BasisTransformation(const Vector3& vector_, const Quartanion& quartanion_, Vector3 origin_)
{
	return BasisTransformation(vector_, quartanion_.GetForward(), quartanion_.GetUp(), quartanion_.GetRight(), origin_);
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

float Vector3::GetComponent(int index_)
{
	switch (index_)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		return 0.0f;
	}
}

Vector3 Vector3::Normalize() const
{
	// 0割りはオペレータでケア
	return *this / Length();
}

float Vector3::Length() const
{
	return sqrtf(sqrLength());
}

float Vector3::sqrLength() const
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

VECTOR Vector3::ToVECTOR() const
{
	VECTOR vector = VGet(x, y, z);
	return vector;
}

DirectX::XMFLOAT3 Vector3::ToXMFLOAT3() const
{
	DirectX::XMFLOAT3 xmfloat3 = { x, y ,z };
	return xmfloat3;
}

void Vector3::ToArray(float array_[COMPONENTS_NUM])
{
	// サイズチェック
	if (sizeof(*array_) / sizeof(float) != COMPONENTS_NUM) { return; }

	array_[0] = x;
	array_[1] = y;
	array_[2] = z;
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

bool operator!=(const Vector3& a, const Vector3& b)
{
	return !(a == b);
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


