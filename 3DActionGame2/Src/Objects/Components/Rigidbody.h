#pragma once
#include "Transform.h"
#include "ComponentBase.h"

#include <memory>

// 便宜上 Rigidbody (剛体) という名前を付けているが
// 扱い的には Mass point (質点)
// 現状、物理演算で回転させる予定がないので
class Rigidbody : public ComponentBase
{


public:
	float Mass;
	Vector3 Velocity = Vector3::ZERO;

	bool UseGravity = true;

public:
	void AddForce(Vector3 power_, float time_);

	void FixedUpdate();

public:
	Rigidbody(float mass_);
	~Rigidbody();
private:
	// 現在加わっている力
	Vector3 force = Vector3::ZERO;
	Vector3 acceleration = Vector3::ZERO;
};