#pragma once
#include "ObjectBase.h"
#include <memory>
#include "Components/Transform.h"

class SimpleObserver;

class SampleCamera : public ObjectBase
{
public:
	SampleCamera(Vector3 position_);
public:
	Vector3 GetNearShadowAreaPos();
	Vector3 GetForward();
	Vector3 GetRight();
	Tag GetTag() { return Tag::Camera; }

public:
	void Start() override {}

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;
	void RenderShadow() override;

	void SetMonochrome(float rate_) override {}

	void SetTarget(Transform* target_);

	void SetCameraInfo();

private:

private:
	// transformの扱いが通常のオブジェクトと違う
	// transform.Position   : 注視点の座標を補正
	// transform.quartanion : カメラの位置(注視点に対してカメラがある方向) 
	Transform transform;
	Transform* target = nullptr;

	float distance = 30.0f;
	float horizontalSpeed = 50.0f;
	float varticalSpeed = 30.0f;

private:
	std::shared_ptr<SimpleObserver> so;
};
