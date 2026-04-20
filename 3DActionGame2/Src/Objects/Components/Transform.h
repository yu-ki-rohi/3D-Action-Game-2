#pragma once
#include "../../Mathmatics/Vector3.h"
#include "../../Mathmatics/Quaternion.h"
#include "../../Common.h"
#include <memory>
#include <vector>
#include "ComponentBase.h"

// TODO: 階層構造の実装(優先度:低)

class Transform : public ComponentBase
{
public:
	Vector3 Position = Vector3::ZERO;
	Vector3 Scale = Vector3::ONE;

	// いずれ階層構造を実装するとき用
	//std::weak_ptr<Transform> Parent;

public:
	// アクセサ
	Vector3 GetRotation() const;

	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetRight() const;

	MATRIX GetTranslateMat() const;
	MATRIX GetRotateMat() const;
	MATRIX GetScaleMat() const;

	const Quaternion& GetQuaternion() const;

public:
	// ワールド空間における上方向を上向きとして回転
	void SetForward(const Vector3& forward_);

	// オイラー角で回転を設定
	void SetRotate(const Vector3& rotate_);

	// 直接代入する必要があるとき
	void SetQuaternion(const Quaternion& quaternion_);

	void StartSlearpByForwardAndDuration(const Vector3& target_forward_, float duration_);
	void StartSlearpByForwardAndAngularVelocity(const Vector3& target_forward_, float angular_velocity);

	// transform行列を用いて位置、回転、拡縮を設定
	void UpdateFromMatrix(const MATRIX& transform_mat_);

	void Update(float elapsed_time_);

public:
	// ワールド座標系の軸による回転
	void RotateAxisX(float angle_);
	void RotateAxisY(float angle_);
	void RotateAxisZ(float angle_);

	// ローカル座標系の右軸による回転
	void RotateAxisRight(float angle_);

	// 自身のTransform情報に従って移動させる
	// (右, 上, 前)
	void Translate(const Vector3& vec_);

#ifdef DEBUG
	void DebugDrow();

#endif

public:
	Transform();
	Transform(Vector3 position_);
	Transform(Vector3 position_, Vector3 scale_);
	Transform(Vector3 position_, Vector3 scale_, Vector3 rotation_);


private:
	void SlearpRotation(float elapsed_time_);

private:
	// オイラー角による回転状態
	// 基本的に参照用なので、こちらを直接変更はしない
	// 単位はラジアン
	Vector3 rotateAngle;

	// こちらが回転情報の本体
	Quaternion quaternion;

	Quaternion slearpInit = Quaternion::IDENTITY;
	Quaternion slearpTarget = Quaternion::IDENTITY;
	float slearpDuration = 1.0f;
	float slearpTime = 0.0f;
	bool isSlearp = false;



	// いずれ階層構造を実装するとき用
	//std::vector<std::shared_ptr<Transform>> children;
};