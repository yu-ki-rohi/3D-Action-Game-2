#pragma once
#include "BoxCollider.h"

// 高速で動くような当たり判定がすり抜けないように加工したBoxCollider
// todo : 判定の張り方の改良
class FlexibleBoxCollider : public BoxCollider
{
public:
	// 回転はオイラー角、単位は度数
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);
	FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, bool is_collide_once_);

public:
	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

private:
	Transform boxTransform;
};