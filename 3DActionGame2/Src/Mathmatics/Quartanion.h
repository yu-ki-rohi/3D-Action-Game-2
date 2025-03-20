#pragma once
// 参考にしたサイトを以下にまとめてあります
// https://docs.google.com/document/d/1MiHupls7hHg30EK4eU9b8I1DqDNullxO5BEPAq3sJFA/edit?tab=t.0#heading=h.53o8674ws84b

struct Vector3;

struct Quartanion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	static const Quartanion IDENTITY;

public:
	static Quartanion GetRotateQuartanion(float degree_, Vector3 axis_);

public:
	Quartanion(float x_, float y_, float z_, float w_);

public:
	Vector3 GetForward();
	Vector3 GetUp();
	Vector3 GetRight();

public:
	Vector3 ToEuler();

public:
	Quartanion operator*(const Quartanion& other);
	Quartanion& operator*=(const Quartanion& other);
};