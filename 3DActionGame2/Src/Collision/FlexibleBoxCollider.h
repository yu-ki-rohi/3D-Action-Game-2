#pragma once
#include "BoxCollider.h"

class FlexibleBoxCollider : public BoxCollider
{
public:
	// 回転はオイラー角、単位は度数
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);

public:
	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Transform boxTransform;
};