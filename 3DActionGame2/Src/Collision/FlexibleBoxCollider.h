#pragma once
#include "BoxCollider.h"

// �����œ����悤�ȓ����蔻�肪���蔲���Ȃ��悤�ɉ��H����BoxCollider
class FlexibleBoxCollider : public BoxCollider
{
public:
	// ��]�̓I�C���[�p�A�P�ʂ͓x��
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, bool is_collide_once_);

public:
	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Transform boxTransform;
};