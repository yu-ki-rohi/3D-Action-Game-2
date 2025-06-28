#pragma once
#include "Collider.h"
#include "../Mathmatics/Vector3.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius_, Vector3 position_);
	SphereCollider(float radius_, std::shared_ptr<ObjectBase> owner_);
	SphereCollider(float radius_, Vector3 position_, std::shared_ptr<ObjectBase> owner_);
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const override;			// 全ての成分が半径であるベクトルを返す
	Vector3 GetRotate() const override;			// 全ての成分が0であるベクトルを返す
	Quaternion GetQuaternion() const override;	// 単位元を返す

	Type GetType() const override;


	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

	void UpdatePosition(const Vector3& new_position_) override;

private:
	Vector3 localPosition;
	Vector3 position;

#ifdef DEBUG
public:
	void DebugDrow() override;
#endif
};