#pragma once
#include <vector>
#include <memory>

class ColliderRegisterInterface;

// �O���錾
class Collider;
struct Vector3;
struct Quartanion;

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
	CollisionManager();
public:
	// ��{�I�ɂ���Ŏ擾�����|�C���^��ێ���������ꍇ��weak_ptr�ɂ���
	std::shared_ptr<ColliderRegisterInterface> GetRegisterInterface();

	void RegisterBody(const Collider* collider_);
	void RegisterTrigger(const Collider* collider_);

	void ReleaseBody(const Collider* collider_);
	void ReleaseTrigger(const Collider* collider_);

	void CheckCollision();

	

private:
	bool IsColliding(const Collider* collider_01_, const Collider* collider_02_);

	bool IsCollidingSphereAndSphere(const Collider* collider_01_, const Collider* collider_02_);
	bool IsCollidingBoxAndSphere(const Collider* box_collider_, const Collider* sphere_collider_);

	// Box(Obb) �� Box(Obb) ���������Ă��邩�̊m�F
	// �����A�����W�����Ă͂�����̂́A�قڐ搶���璸�����\�[�X���g�p���Ă��܂�
	bool IsCollidingBoxAndBox(const Collider* collider_01_, const Collider* collider_02_);

	// IsCollidingBoxAndBox�̒��ŌĂяo�����֐�
	bool IsFindOBBSparationAxis(const Vector3& axis_, Vector3 vertices_01_[8], Vector3 vertices_02_[8]);
	bool CheckOBBLoacalAxisSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8]);
	bool CheckOBBCrossVecSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8]);

	// �O�̃t���[���œ������Ă�����
	// �I�v�V�����Ƃ��āA��O������true��n���ƊY���̃q�b�g�L�^���폜�ł���
	bool WasCollided(const Collider* collider_01_, const Collider* collider_02_,bool does_erase_ = false);

private:
	// Collider�̓o�^�E�������s���C���^�[�t�F�[�X
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	std::vector<const Collider*> bodies;
	std::vector<const Collider*> triggers;
	std::vector<std::pair<const Collider*, const Collider*>> preCollided;
};