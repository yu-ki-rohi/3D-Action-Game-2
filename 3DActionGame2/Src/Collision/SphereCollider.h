#pragma once
#include "Collider.h"
#include "../Mathmatics/Vector3.h"

class SphereCollider : public Collider
{
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const override;			// 全ての成分が半径であるベクトルを返す
	Vector3 GetRotate() const override;			// 全ての成分が0であるベクトルを返す
	Quartanion GetQuartanion() const override;	// 単位元を返す

	float GetRadius() const override;

	Type GetType() const override;


	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Vector3 position;

};