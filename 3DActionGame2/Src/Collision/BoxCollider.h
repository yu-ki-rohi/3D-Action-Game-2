#pragma once
#include "Collider.h"
#include "../Objects/Components/Transform.h"

class BoxCollider : public Collider
{
public:
	// 回転はオイラー角、単位は度数
	BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);
	BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, std::shared_ptr<ObjectBase> owner_);
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const  override;
	Vector3 GetRotate() const override;
	Quaternion GetQuaternion() const override;

	Type GetType() const override;

	virtual void UpdateFromParentMat(const MATRIX& parent_mat_) override;

	void UpdatePosition(const Vector3& new_position_) override;

	virtual void UpdateRadius();

protected:
	// 親オブジェクトに対するTransform
	Transform localTransform;
	// World空間におけるTransform
	Transform transform;

#ifdef DEBUG
public:
	void DebugDrow() override;
#endif

};
