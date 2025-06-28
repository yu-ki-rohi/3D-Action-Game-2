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

	// ベクトルの基底をワールド座標系軸から任意の座標系軸へ変換して出力
	// 第一引数     変換したいベクトル
	// 第二～四引数 変換先の基底(軸) X, Y, Z の順　(または右、上、前)
	// 第五引数     原点座標(デフォルトではワールド座標系の原点)
	static Vector3 BasisTransformation(const Vector3& vector_, Vector3 axis_01_, Vector3 axis_02_, Vector3 axis_03_, Vector3 origin_ = Vector3::ZERO);
	static Vector3 BasisTransformation(const Vector3& vector_, const Quaternion& quaternion_, Vector3 origin_ = Vector3::ZERO);

	static Vector3 ConvertFromVECTOR(const VECTOR& vector_);
	static Vector3 ConvertFromXMFLOTA3(const DirectX::XMFLOAT3& xmfloat3_);

public:
	Vector3();
	Vector3(float x_, float y_, float z_);

public:
	// 成分を取り出す関数
	// 疑似的に配列として使いたいときに
	// 範囲外を指定したときは 0 を返す
	float GetComponent(int index_);

	// 正規化して返す
	Vector3 Normalize() const;
	// 長さを返す
	float Length() const;
	// 長さの二乗を返す
	// 大きさ比較など、わざわざ平方根を計算する必要がないときに使用
	float sqrLength() const;

	// 射影
	Vector3 Projection(Vector3 axis_);
	Vector3 Projection(Vector3 axis_01_, Vector3 axis_02_);

	VECTOR ToVECTOR() const;
	DirectX::XMFLOAT3 ToXMFLOAT3() const;

	// 渡されたfloat型配列に成分を詰め込んで返す
	// 配列に変換したいときに使う
	// 渡す配列の長さは3
	// 長さの指定は原則このクラスのCOMPONENTS_NUMを使う
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
