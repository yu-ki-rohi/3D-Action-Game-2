#pragma once
#include "Player.h"

class PlayerSaber : public Player
{
public:
	PlayerSaber(std::shared_ptr<CameraManager> camera_manager_);

public:
	void Start() override;

	void FixedUpdate() override;
	void Render() override;

protected:
	void UpdateBehavior(float elapsed_time_) override;
	void UpdateCollider() override;

private:
	void IgnitIdleAnimation();
	void IgnitWalkAnimation();
	void IgnitAttackAnimation();
	void IgnitRollAnimation();

private:
	void FinishAttack();

private:
	BoxCollider attackCollider;
	BoxCollider bodyCollider;

private:

};
