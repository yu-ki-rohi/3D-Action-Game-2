#pragma once
#include "ObjectBase.h"

struct Vector3;

class Transform;

class CameraBase : public ObjectBase
{
public:
	enum class Type
	{
		TPS
	};

public:
	Tag GetTag() const override
	{
		return Tag::Camera;
	}

	virtual Type GetType() const = 0;

	virtual Vector3 GetCameraPosition() const = 0;
	virtual Vector3 GetTargetPosition() const = 0;

	virtual Vector3 GetNearShadowAreaPos() const = 0;
	virtual Vector3 GetForward() const = 0;
	virtual Vector3 GetRight() const = 0;


	void SetMonochrome(float rate_) override {}
	void SetLocalTimeScale(float time_scale_) override {}
	void MultiplyLocalTimeScaleBy(float multiplier_) override {}

	void SetTransform(std::shared_ptr<Transform> transform_)
	{
		transform = transform_;
	}

	void SetTarget(std::shared_ptr<Transform> target_)
	{
		target = target_;
	}

	void Render() override {}
	void RenderShadow() override {}

protected:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Transform> target;

private:

};