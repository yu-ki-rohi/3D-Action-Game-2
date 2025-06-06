#pragma once
#include <DxLib.h>

struct Vector3;
struct Quartanion;
class ObjectBase;

class Collider
{
public:
	enum class Type
	{
		Sphere,
		Box
	};

public:
	Collider() = default;
	Collider(ObjectBase* owner_) : owner(owner_) {}

public:
	// アクセサ
	virtual Vector3 GetPosition() const = 0;
	virtual Vector3 GetScale() const = 0;
	virtual Vector3 GetRotate() const = 0;
	virtual Quartanion GetQuartanion() const = 0;

	float GetRadius() const { return radius; }

	// OBBか球かの判定用
	virtual Type GetType() const = 0;

	ObjectBase* GetOwner() const
	{
		return owner;
	}

	// 親オブジェクトの行列によるTransformの更新
	virtual void UpdateFromParentMat(const MATRIX& parent_mat_) = 0;

	virtual void UpdatePosition(const Vector3& new_position_) = 0;

protected:
	// コライダーの所有者
	// 自身との当たりや複数回ヒットすることを防ぐため
	ObjectBase* owner = nullptr;

	// 球以外の場合は、その当たり判定を包含する球の半径(Bounding Sphere)
	float radius = 1.0f;
};