#pragma once
#include "BoxCollider.h"

class FlexibleBoxCollider : public BoxCollider
{
public:
	// ��]�̓I�C���[�p�A�P�ʂ͓x��
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);

public:
	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Transform boxTransform;
};