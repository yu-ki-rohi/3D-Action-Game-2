#pragma once
#include "../../Mathmatics/Vector3.h"
#include "../../Mathmatics/Quartanion.h"
#include "../../Common.h"
#include <memory>
#include <vector>
#include "ComponentBase.h"

// Suggested (�R���|�[�l���g�S�ʂɊւ�����̂����A��\�Ƃ��Ă����ɕ\�L)
// ���֐���������Ȃ�ComponentBase�N���X�����GetComponent�֐������
// (�_�E���L���X�g���������Ă��܂���)

// TODO
// �K�w�\���̎���(�D��x:��)

struct Transform : public ComponentBase
{
public:
	Vector3 Position = Vector3::ZERO;
	Vector3 Scale = Vector3::ONE;

	// ������K�w�\������������Ƃ��p
	//std::weak_ptr<Transform> Parent;

public:
	// �A�N�Z�T
	Vector3 GetRotation() const;

	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetRight() const;

	MATRIX GetTranslateMat() const;
	MATRIX GetRotateMat() const;
	MATRIX GetScaleMat() const;

	const Quartanion& GetQuartanion() const;

public:
	// ���[���h��Ԃɂ�����������������Ƃ��ĉ�]
	void SetForward(const Vector3& forward_);

	// �I�C���[�p�ŉ�]��ݒ�
	void SetRotate(const Vector3& rotate_);

	// transform�s���p���Ĉʒu�A��]�A�g�k��ݒ�
	void UpdateFromMatrix(const MATRIX& transform_mat_);

public:
	// ���[���h���W�n�̎��ɂ���]
	void RotateAxisX(float angle_);
	void RotateAxisY(float angle_);
	void RotateAxisZ(float angle_);

	// ���[�J�����W�n�̉E���ɂ���]
	void RotateAxisRight(float angle_);

	// ���g��Transform���ɏ]���Ĉړ�������
	// (�E, ��, �O)
	void Translate(const Vector3& vec_);

#ifdef DEBUG
	void DebugDrow();

#endif

public:
	Transform() = default;
	Transform(Vector3 position_);
	Transform(Vector3 position_, Vector3 scale_);

private:
	// �I�C���[�p�ɂ���]���
	// ��{�I�ɎQ�Ɨp�Ȃ̂ŁA������𒼐ڕύX�͂��Ȃ�
	// �P�ʂ̓��W�A��
	Vector3 rotateAngle = Vector3::ZERO;

	// �����炪��]���̖{��
	Quartanion quartanion = Quartanion::IDENTITY;

	// ������K�w�\������������Ƃ��p
	//std::vector<std::shared_ptr<Transform>> children;
};