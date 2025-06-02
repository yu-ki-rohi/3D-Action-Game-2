#pragma once
#include "Collider.h"
#include "../Mathmatics/Vector3.h"

class SphereCollider : public Collider
{
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const override;			// �S�Ă̐��������a�ł���x�N�g����Ԃ�
	Vector3 GetRotate() const override;			// �S�Ă̐�����0�ł���x�N�g����Ԃ�
	Quartanion GetQuartanion() const override;	// �P�ʌ���Ԃ�

	float GetRadius() const override;

	Type GetType() const override;


	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Vector3 position;

};