#pragma once
#include "Transform.h"

// ‹@”\‚ª‘‚¦‚é‚È‚çclass‚É‚µ‚½•û‚ª®‡«‚ªã‚ª‚é

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
	// Œ»İ‰Á‚í‚Á‚Ä‚¢‚é—Í
	Vector3 force = Vector3::ZERO;
};