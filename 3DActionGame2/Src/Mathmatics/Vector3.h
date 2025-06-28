#pragma once

#include <DxLib.h>
#include <DirectXMath.h>

struct Quaternion;

struct Vector3
{
public:
	float x;
	float y;
	float z;

public:
	static const Vector3 FORWARD;
	static const Vector3 UP;
	static const Vector3 RIGHT;
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static constexpr int COMPONENTS_NUM = 3;

public:
	static float Dot(const Vector3& a, const Vector3& b);
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	// �x�N�g���̊������[���h���W�n������C�ӂ̍��W�n���֕ϊ����ďo��
	// ������     �ϊ��������x�N�g��
	// ���`�l���� �ϊ���̊��(��) X, Y, Z �̏��@(�܂��͉E�A��A�O)
	// ��܈���     ���_���W(�f�t�H���g�ł̓��[���h���W�n�̌��_)
	static Vector3 BasisTransformation(const Vector3& vector_, Vector3 axis_01_, Vector3 axis_02_, Vector3 axis_03_, Vector3 origin_ = Vector3::ZERO);
	static Vector3 BasisTransformation(const Vector3& vector_, const Quaternion& quaternion_, Vector3 origin_ = Vector3::ZERO);

	static Vector3 ConvertFromVECTOR(const VECTOR& vector_);
	static Vector3 ConvertFromXMFLOTA3(const DirectX::XMFLOAT3& xmfloat3_);

public:
	Vector3();
	Vector3(float x_, float y_, float z_);

public:
	// ���������o���֐�
	// �^���I�ɔz��Ƃ��Ďg�������Ƃ���
	// �͈͊O���w�肵���Ƃ��� 0 ��Ԃ�
	float GetComponent(int index_);

	// ���K�����ĕԂ�
	Vector3 Normalize() const;
	// ������Ԃ�
	float Length() const;
	// �����̓���Ԃ�
	// �傫����r�ȂǁA�킴�킴���������v�Z����K�v���Ȃ��Ƃ��Ɏg�p
	float sqrLength() const;

	// �ˉe
	Vector3 Projection(Vector3 axis_);
	Vector3 Projection(Vector3 axis_01_, Vector3 axis_02_);

	VECTOR ToVECTOR() const;
	DirectX::XMFLOAT3 ToXMFLOAT3() const;

	// �n���ꂽfloat�^�z��ɐ������l�ߍ���ŕԂ�
	// �z��ɕϊ��������Ƃ��Ɏg��
	// �n���z��̒�����3
	// �����̎w��͌������̃N���X��COMPONENTS_NUM���g��
	void ToArray(float array_[COMPONENTS_NUM]);

public:
	friend Vector3 operator+(const Vector3& a, const Vector3& b);
	friend Vector3 operator-(const Vector3& a, const Vector3& b);
	friend bool operator==(const Vector3& a, const Vector3& b);
	friend bool operator!=(const Vector3& a, const Vector3& b);
	friend Vector3 operator+(const Vector3& a, const float& b);
	friend Vector3 operator-(const Vector3& a, const float& b);
	friend Vector3 operator*(const Vector3& a, const float& b);
	friend Vector3 operator/(const Vector3& a, float b);
	friend Vector3 operator+(const float& a, const Vector3& b);
	friend Vector3 operator-(const float& a, const Vector3& b);
	friend Vector3 operator*(const float& a, const Vector3& b);
	friend Vector3 operator-(const Vector3& a);
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator+=(const float& other);
	Vector3& operator-=(const float& other);
	Vector3& operator*=(const float& other);
};
