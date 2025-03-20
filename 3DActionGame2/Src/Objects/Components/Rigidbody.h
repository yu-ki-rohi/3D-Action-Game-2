#pragma once
#include "Transform.h"

struct Rigidbody
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
	// åªç›â¡ÇÌÇ¡ÇƒÇ¢ÇÈóÕ
	Vector3 force = Vector3::ZERO;
};