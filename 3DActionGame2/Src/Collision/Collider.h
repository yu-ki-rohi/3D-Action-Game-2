#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Objects/Components/ComponentBase.h"
#include "../Common.h"
#include "../Mathmatics/Vector3.h"

struct Quaternion;
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
	Collider() : radius(1.0f), isEnabled(true), hitPosition(Vector3::ZERO) {}
	Collider(float radius_) : 	radius(radius_), isEnabled(true), hitPosition(Vector3::ZERO) {}
	// �U������ȂǁA��������ɘA���ł͓�����Ȃ��悤�ɂ������ꍇ�͑�������true��n��
	Collider(float radius_, bool is_collide_once_) : 	radius(radius_), isEnabled(true), hitPosition(Vector3::ZERO)
	{
		if (is_collide_once_)
		{
			haveHitObjects = std::make_unique<HaveHitObjects>();
		}
	}

public:
	// �A�N�Z�T
	virtual Vector3 GetPosition() const = 0;
	virtual Vector3 GetScale() const = 0;
	virtual Vector3 GetRotate() const = 0;
	virtual Quaternion GetQuaternion() const = 0;
	virtual Type GetType() const = 0;

	float GetRadius() const { return radius; }

	Vector3 GetHitPosition() const { return hitPosition; }

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

	void SetHitPosition(Vector3 hit_position_)
	{
		hitPosition = hit_position_;
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

	void ClearHasHit()
	{
		if (!haveHitObjects) { return; }
		haveHitObjects->clear();
	}

	// �e�퓖���莞�ɌĂяo��
	void OnTriggerEnter(Collider* other_)
	{
		if (HasHit(other_)) { return; }
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
		if (HasHit(other_)) { return; }
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
	
private:
	bool HasHit(Collider* other_)
	{
		if (!haveHitObjects) { return false; }
		auto other_owner = other_->GetOwner();
		if (!other_owner) { return false; }

		for (auto hit_obj : *haveHitObjects)
		{
			auto hit_object = hit_obj.lock();
			if (hit_object == other_owner) { return true; }
		}

		haveHitObjects->push_back(other_owner);
		return false;
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

	using HaveHitObjects = std::vector<std::weak_ptr<ObjectBase>>;
	// ���ɓ������Ă���I�u�W�F�N�g
	std::unique_ptr<HaveHitObjects> haveHitObjects;

	Vector3 hitPosition;

#ifdef DEBUG
public:
	virtual void DebugDrow() = 0;
#endif
};