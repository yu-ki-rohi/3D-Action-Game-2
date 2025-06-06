#pragma once
#include "Transform.h"
#include "ComponentBase.h"

// 便宜上 Rigid body (剛体) という名前を付けてますが、
// 扱い的には Mass point (質点) です。
// 現状、物理演算で回転させる予定がないので
class Rigidbody : public ComponentBase
{
public:
	float Mass;
	Vector3 Velocity = Vector3::ZERO;

	bool UseGravity = true;

public:
	void AddForce(Vector3 power_, float time_);

public:
	Rigidbody(float mass_);
	~Rigidbody();
private:
	// 現在加わっている力
	Vector3 force = Vector3::ZERO;
};