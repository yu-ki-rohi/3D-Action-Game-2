#pragma once
#include <vector>
#include <memory>

class ColliderRegisterInterface;

// �O���錾
class Collider;

/*
	�_���[�W����������
	A.
	GetComponent��p�ӂ��ă^�O�t�����Ċ֐����s
	���_
	�ėp��������
	Unity���C�N�̃R���Z�v�g�ɉ���
	���_
	dynamic_cast�̔����ɂ����S���E�������ԓ��̖��

	B.
	CharacterBase�ɔh��������
	���_
	C++
	���_
	�ÓI�I�u�W�F�N�g�Ƌ��ʉ��ł��Ȃ��Ȃ�

	C.
	DataManager�I�ȃN���X��p�ӂ�������o�R�ŏ����擾
	���_

	���_


*/


class CollisionManager : public std::enable_shared_from_this<CollisionManager>
{
public:
	// ��{�I�ɂ���Ŏ擾�����|�C���^��ێ���������ꍇ��weak_ptr�ɂ���
	std::shared_ptr<ColliderRegisterInterface> GetRegisterInterface();

	void CheckCollision();

private:
	bool IsCollidingSphereAndSphere(const Collider* collider_01_, const Collider* collider_02_);
	bool IsCollidingBoxAndSphere(const Collider* box_collider_, const Collider* sphere_collider_);

	// ���ݎ�����
#if false

	// Box(Obb) �� Box(Obb) ���������Ă��邩���m�F
	// �����A�����W�����Ă͂�����̂́A�قڐ搶���璸�����\�[�X���g�p���Ă��܂�
	bool IsCollidingBoxAndBox(const Collider* collider_01_, const Collider* collider_02_);


private:
	// IsCollidingBoxAndBox�̒��ŌĂяo�����֐�
	bool IsFindOBBSparationAxis(const VECTOR& axis_, VECTOR vertices_01_[8], VECTOR vertices_02_[8]);
	bool CheckOBBLoacalAxisSAT(Axis axes_list_[2], VECTOR vertices_list_[2][8]);
	bool CheckOBBCrossVecSAT(Axis axes_list_[2], VECTOR vertices_list_[2][8]);

#endif
private:
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	std::vector<Collider*> bodies;
	std::vector<Collider*> triggers;
};