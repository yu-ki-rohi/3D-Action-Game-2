#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Objects/Components/ComponentBase.h"

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
	Collider() = default;
	Collider(ObjectBase* owner_) : owner(owner_) {}

public:
	// �A�N�Z�T
	virtual Vector3 GetPosition() const = 0;
	virtual Vector3 GetScale() const = 0;
	virtual Vector3 GetRotate() const = 0;
	virtual Quartanion GetQuartanion() const = 0;

	float GetRadius() const { return radius; }

	// OBB�������̔���p
	virtual Type GetType() const = 0;

	ObjectBase* GetOwner() const
	{
		return owner;
	}


	// �e�I�u�W�F�N�g�̍s��ɂ��Transform�̍X�V
	virtual void UpdateFromParentMat(const MATRIX& parent_mat_) = 0;

	virtual void UpdatePosition(const Vector3& new_position_) = 0;

	// �����莞�ɒʒm����Ώۂ�ǉ�
	void AddObserver(std::shared_ptr<ComponentBase> observer_)
	{
		observers.push_back(observer_);
	}

	// �e�퓖���莞�ɌĂяo��
	void OnTriggerEnter(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnTriggerEnter(other_);
		}
	}
	void OnTriggerStay(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnTriggerStay(other_);
		}
	}
	void OnTriggerExit(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnTriggerExit(other_);
		}
	}

	void OnCollisionEnter(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnCollisionEnter(other_);
		}
	}
	void OnCollisionStay(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnCollisionStay(other_);
		}
	}
	void OnCollisionExit(const Collider* other_) const
	{
		for (auto obeserver : observers)
		{
			obeserver->OnCollisionExit(other_);
		}
	}

protected:
	// �R���C�_�[�̏��L��
	// ���g�Ƃ̓�����╡����q�b�g���邱�Ƃ�h������
	ObjectBase* owner = nullptr;

	// ���ȊO�̏ꍇ�́A���̓����蔻����܂��鋅�̔��a(Bounding Sphere)
	float radius = 1.0f;

private:
	// �������ʒm���鑊��
	std::vector<std::shared_ptr<ComponentBase>> observers;
};