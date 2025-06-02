#pragma once
#include "Collider.h"
#include "../Objects/Components/Transform.h"

class BoxCollider : public Collider
{
public:
	// 回転はオイラー角、単位は度数
	BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const  override;
	Vector3 GetRotate() const override;
	Quartanion GetQuartanion() const override;

	// ※中心から角(最も遠い位置)までの距離を半径として扱う
	float GetRadius() const override;

	Type GetType() const override;

	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

#ifdef DEBUG
	void DebugDrow();
#endif

private:
	// 親オブジェクトに対するTransform
	Transform localTransform;
	// World空間におけるTransform
	Transform transform;
};
