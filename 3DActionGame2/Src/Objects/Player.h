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
	// 回転回避の挙動段階
	// -1:回転回避状態ではない
	//  0:飛び込み状態
	//  1:着地状態
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