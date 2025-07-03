#pragma once
#include "CameraBase.h"

class SimpleObserver;

class CameraTPS : public CameraBase
{
public:
	CameraTPS();

public:
	CameraBase::Type GetType() const override;

	Vector3 GetCameraPosition() const override;
	Vector3 GetTargetPosition() const override;

	Vector3 GetNearShadowAreaPos() const override;
	Vector3 GetForward() const override;
	Vector3 GetRight() const override;

	void Start() override;

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;


private:
	std::shared_ptr<SimpleObserver> so;
	bool isReverseVertical;
	bool isReverseHorizontal;
};