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
	void IgnitAttackAnimation();

private:
	void FinishAttack();
	void ResetAttackStep();

private:
	FlexibleBoxCollider attackCollider;
	BoxCollider bodyCollider;

	char attackStep;

private:

};
