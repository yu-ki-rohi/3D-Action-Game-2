#pragma once
#include <DxLib.h>

struct Vector3;
struct Quartanion;
class ObjectBase;

class Collider
{
public:
	enum class Type
	{
		Sphere,
		Box
	};

public:
	// �A�N�Z�T
	virtual Vector3 GetPosition() const = 0;
	virtual Vector3 GetScale() const = 0;
	virtual Vector3 GetRotate() const = 0;
	virtual Quartanion GetQuartanion() const = 0;

	virtual float GetRadius() const = 0;

	// OBB�������̔���p
	virtual Type GetType() const = 0;

	ObjectBase* GetOwner() const
	{
		return owner;
	}

	// �e�I�u�W�F�N�g�̍s��ɂ��Transform�̍X�V
	virtual void UpdateFromParentMat(const MATRIX& parent_mat_) = 0;

protected:
	// �R���C�_�[�̏��L��
	// ���g�Ƃ̓�����╡����q�b�g���邱�Ƃ�h������
	ObjectBase* owner = nullptr;

	// ���̓����蔻����܂��鋅�̔��a
	float radius;
};