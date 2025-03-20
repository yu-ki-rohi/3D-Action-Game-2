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
	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void SetTarget(Transform* target_);

	void SetCameraInfo();

private:

private:
	// transform�̈��������I�u�W�F�N�g�ƈႤ
	// transform.Position   : �����_�̍��W��␳
	// transform.quartanion : �J�����̈ʒu(�����_�ɑ΂��ăJ�������������) 
	Transform transform;
	Transform* target = nullptr;

	float distance = 30.0f;
	float horizontalSpeed = 50.0f;
	float varticalSpeed = 30.0f;

private:
	std::shared_ptr<SimpleObserver> so;
};
