#pragma once
#include "ObjectBase.h"

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
	void SetLocalTimeScale(float time_scale_) override;


	// Startが呼び出されるまでに以下のポインタが指す実体がない場合、そのObjectは破棄
	// transform, renderer, animator
	virtual void Start() override;

	virtual void FixedUpdate() override;
	virtual void Update(float elapsed_time_) override;
	virtual void Render() override;
	void RenderShadow() override;

protected:
	virtual void UpdateBehavior(float elapsed_time_) = 0;
	virtual void UpdateCollider() = 0;

protected:

	std::shared_ptr<Transform> transform;

	std::shared_ptr <Renderer> renderer;
	std::shared_ptr <Animator> animator;
	std::shared_ptr <CharacterStatus> characterStatus;

	std::weak_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	float monochromeRate;
	float localTimeScale;
};