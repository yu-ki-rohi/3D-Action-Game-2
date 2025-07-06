#pragma once
#include "CharacterBase.h"
#include "../Collision/BoxCollider.h"

class JustAvoidIgnition;
class TimerBase;

class Enemy : public CharacterBase
{
public:
	Enemy();

public:
	Tag GetTag() const override;

	virtual void SetLocalTimeScale(float time_scale_) override;
	virtual void MultiplyLocalTimeScaleBy(float multiplier_) override;

	void Start() override;

	void FixedUpdate() override;
	void Render() override;

	

protected:
	virtual void UpdateBehavior(float elapsed_time_) override;
	void UpdateCollider() override;

private:
	void EnableAttackCollider();
	void DisableAttackCollider();

private:
	BoxCollider attackCollider;
	BoxCollider bodyCollider;
	BoxCollider justAvoidIgnitionCollider;

	std::shared_ptr<JustAvoidIgnition> justAvoidIgnition;

	std::shared_ptr<TimerBase> enableColliderTimer;
	std::shared_ptr<TimerBase> disableColliderTimer;

	bool isChanging;
};