#pragma once
#include "Player.h"
#include <unordered_map>

class TimerBase;

class PlayerSaber : public Player
{
public:
	PlayerSaber(std::shared_ptr<CameraManager> camera_manager_);

public:
	void Start() override;

	void FixedUpdate() override;
	void Render() override;

	void HitStop() override;

protected:
	void UpdateBehavior(float elapsed_time_) override;
	void UpdateCollider() override;

	void FinishHitStop() override;

private:
	void IgnitAttackAnimation();

private:
	void EnableAttackCollider();
	void DisableAttackCollider();
	void FinishAttack();
	void ResetAttackStep();

private:
	FlexibleBoxCollider attackCollider;
	BoxCollider bodyCollider;

	char attackStep;

	enum class TimerKind
	{
		EnableAttackCollider,
		DisableAttackCollider,
		FinishAttack,
		ResetAttackStep,
	};
	std::unordered_map<TimerKind, MyTimer> myTimers;
	

private:

};
