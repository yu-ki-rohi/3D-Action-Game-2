#pragma once
#include "Collider.h"
#include "../Objects/Components/Transform.h"

class BoxCollider : public Collider
{
public:
	// ��]�̓I�C���[�p�A�P�ʂ͓x��
	BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_);
	BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, std::shared_ptr<ObjectBase> owner_);
public:
	Vector3 GetPosition() const override;
	Vector3 GetScale() const  override;
	Vector3 GetRotate() const override;
	Quartanion GetQuartanion() const override;

	// �����S����p(�ł������ʒu)�܂ł̋����𔼌a�Ƃ��Ĉ���
	//float GetRadius() const;

	Type GetType() const override;

	void UpdateFromParentMat(const MATRIX& parent_mat_) override;

	void UpdatePosition(const Vector3& new_position_) override;

#ifdef DEBUG
	void DebugDrow();
#endif

private:
	// �e�I�u�W�F�N�g�ɑ΂���Transform
	Transform localTransform;
	// World��Ԃɂ�����Transform
	Transform transform;
};
