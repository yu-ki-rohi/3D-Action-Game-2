#pragma once
#include "CharacterBase.h"
#include "../Collision/FlexibleBoxCollider.h"

class CameraManager;
class SimpleObserver;

class Player : public CharacterBase
{
public:
	Player(std::shared_ptr<CameraManager> camera_manager_);
	~Player() = default;

public:
	Tag GetTag() const override;

	virtual void Start() override;

	virtual void HitStop();

protected:

	virtual void UpdateBehavior(float elapsed_time_) override;


	void IgnitIdleAnimation();
	void IgnitWalkAnimation();
	void IgnitRolling();

	virtual void FinishHitStop();

private:
	void ProceedToNextRollingStep();
	void FinishRolling();

protected:
	bool canMove;
	// ‰ñ“]‰ñ”ğ‚Ì‹““®’iŠK
	// -1:‰ñ“]‰ñ”ğó‘Ô‚Å‚Í‚È‚¢
	//  0:”ò‚Ñ‚İó‘Ô
	//  1:’…’nó‘Ô
	char rollingStep;
	std::shared_ptr<Vector3> rollingDirection;

	std::shared_ptr<SimpleObserver> so;
private:
	std::weak_ptr<CameraManager> cameraManager;

private:
	static constexpr float hitStopTimeScale = 0.01f;
	static constexpr float hitStopTimeScaleInv = 1.0f / hitStopTimeScale;
	static constexpr float hitStopTime = 0.15f;

};