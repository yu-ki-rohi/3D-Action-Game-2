#pragma once

#include <DxLib.h>
#include <DirectXMath.h>

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


public:
	static float Dot(const Vector3& a, const Vector3& b);
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	static Vector3 ConvertFromVECTOR(const VECTOR& vector_);
	static Vector3 ConvertFromXMFLOTA3(const DirectX::XMFLOAT3& xmfloat3_);

public:
	Vector3();
	Vector3(float x_, float y_, float z_);

public:
	Vector3 Normalize();
	float Length();
	float sqrLength();

	// ŽË‰e
	Vector3 Projection(Vector3 axis_);
	Vector3 Projection(Vector3 axis_01_, Vector3 axis_02_);

	VECTOR ToVECTOR();
	DirectX::XMFLOAT3 ToXMFLOAT3();

public:
	friend Vector3 operator+(const Vector3& a, const Vector3& b);
	friend Vector3 operator-(const Vector3& a, const Vector3& b);
	friend bool operator==(const Vector3& a, const Vector3& b);
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
