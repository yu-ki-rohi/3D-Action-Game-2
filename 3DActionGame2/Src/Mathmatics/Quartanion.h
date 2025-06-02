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
	// 単位元 (つまりクォータニオンにおいて、実数における1に相当するもの)
	static const Quartanion IDENTITY;

public:
	// 任意軸で回転させるクォータニオンを返す
	// 角度の単位は度数
	static Quartanion GetRotateQuartanion(float degree_, Vector3 axis_);

	// オイラー角からクォータニオンへの変換
	// 回転順はXYZ
	static Quartanion ConvertFromEular(const Vector3& eular_);

	// 回転行列からクォータニオンへの変換
	// 回転行列は列ベクトルを3つ並べたものとして扱う
	static Quartanion ConvertFrom3x3Matrix(const Vector3& col_00_, const Vector3& col_01_, const Vector3& col_02_);

public:
	Quartanion(float x_, float y_, float z_, float w_);

public:
	// クォータニオンから回転行列の一部へ変換
	// それぞれ前、上、右方向のベクトルを返す
	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetRight() const;

public:
	// オイラー角への変換
	Vector3 ToEuler() const;

public:
	Quartanion operator*(const Quartanion& other);
	Quartanion& operator*=(const Quartanion& other);
};