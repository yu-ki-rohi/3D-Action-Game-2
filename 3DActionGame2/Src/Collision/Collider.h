#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Objects/Components/ComponentBase.h"
#include "../Common.h"

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
	Collider() : radius(1.0f), isEnabled(true) {}
	Collider(float radius_) : 	radius(radius_), isEnabled(true) {}
	Collider(float radius_, std::shared_ptr<ObjectBase> owner_) : 	radius(radius_), isEnabled(true), owner(owner_) {}

public:
	// �A�N�Z�T
	virtual Vector3 GetPosition() const = 0;
	virtual Vector3 GetScale() const = 0;
	virtual Vector3 GetRotate() const = 0;
	virtual Quartanion GetQuartanion() const = 0;
	virtual Type GetType() const = 0;

	float GetRadius() const { return radius; }
	bool IsEnabled() const { return isEnabled; }
	std::shared_ptr<ObjectBase> GetOwner() const { return owner.lock(); }

	void SetIsEnabled(bool is_enabled_)
	{
		isEnabled = is_enabled_;
	}

	void SetOwner(std::shared_ptr<ObjectBase> owner_)
	{
		owner = owner_;
	}


	// �e�I�u�W�F�N�g�̍s��ɂ��Transform�̍X�V
	virtual void UpdateFromParentMat(const MATRIX& parent_mat_) = 0;
	// ���W�ɂ��ʒu�X�V
	virtual void UpdatePosition(const Vector3& new_position_) = 0;

	// �����莞�ɒʒm����Ώۂ�ǉ�
	void AddObserver(std::shared_ptr<ComponentBase> observer_)
	{
		observers.push_back(observer_);
	}

	// �e�퓖���莞�ɌĂяo��
	void OnTriggerEnter(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnTriggerEnter(other_);
		}
	}
	void OnTriggerStay(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnTriggerStay(other_);
		}
	}
	void OnTriggerExit(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnTriggerExit(other_);
		}
	}

	void OnCollisionEnter(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnCollisionEnter(other_);
		}
	}
	void OnCollisionStay(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnCollisionStay(other_);
		}
	}
	void OnCollisionExit(Collider* other_)
	{
		for (auto& obeserver : observers)
		{
			obeserver->OnCollisionExit(other_);
		}
	}

protected:

	// ���ȊO�̏ꍇ�́A���̓����蔻����܂��鋅�̔��a(Bounding Sphere)
	float radius;

	bool isEnabled;

	// �R���C�_�[�̏��L��
	std::weak_ptr<ObjectBase> owner;
private:
	// �������ʒm���鑊��
	std::vector<std::shared_ptr<ComponentBase>> observers;


#ifdef DEBUG
public:
	virtual void DebugDrow() = 0;
#endif
};