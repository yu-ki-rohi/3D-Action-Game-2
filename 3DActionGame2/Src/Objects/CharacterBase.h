#pragma once
#include "ObjectBase.h"
#include "../Systems/TimerFactory.h"

struct Vector3;

class Transform;
class Renderer;
class Animator;
class CharacterStatus;

//class Collider;
class ColliderRegisterInterface;

class CharacterBase : public ObjectBase
{
public:
	CharacterBase();
	~CharacterBase() = default;

public:
	Vector3 GetPosition();

	void SetComponent(std::shared_ptr<Transform> transform_);
	void SetComponent(std::shared_ptr<Renderer> renderer_);
	void SetComponent(std::shared_ptr<Animator> animator_);
	void SetComponent(std::shared_ptr <CharacterStatus> characterStatus_);
	void SetColliderRegisterInterface(std::shared_ptr<ColliderRegisterInterface> collider_register_interface_);

	void SetMonochrome(float rate_) override;
	virtual void SetLocalTimeScale(float time_scale_) override;
	virtual void MultiplyLocalTimeScaleBy(float multiplier_) override;


	// Start���Ăяo�����܂łɈȉ��̃|�C���^���w�����̂��Ȃ��ꍇ�A����Object�͔j��
	// transform, renderer, animator
	virtual void Start() override;

	virtual void FixedUpdate() override;
	virtual void Update(float elapsed_time_) override;
	virtual void Render() override;
	void RenderShadow() override;

protected:
	virtual void UpdateBehavior(float elapsed_time_) = 0;
	virtual void UpdateCollider() = 0;

	using MyTimer = std::shared_ptr<TimerBase>;
	template <class T>
	void PrepareTimer(MyTimer& my_timer_,float time_, T* obj_, void (T::* func_)())
	{
		// my_timer_��nullptr�łȂ��ꍇ�����t���O��؂�
		if (my_timer_)
		{
			my_timer_->Erase();
		}
		my_timer_ = TimerFactory::CreateTimer(time_, shared_from_this(), obj_, func_);

		my_timer_->SetLocalTimeScale(localTimeScale);
	}

protected:

	std::shared_ptr<Transform> transform;

	std::shared_ptr <Renderer> renderer;
	std::shared_ptr <Animator> animator;
	std::shared_ptr <CharacterStatus> characterStatus;

	std::weak_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	float monochromeRate;
	float localTimeScale;
};