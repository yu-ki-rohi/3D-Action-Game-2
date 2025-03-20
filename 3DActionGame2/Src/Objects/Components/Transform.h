#pragma once
#include "../../Mathmatics/Vector3.h"
#include "../../Mathmatics/Quartanion.h"
#include "../../Common.h"
#include <memory>
#include <vector>

// TODO
// äKëwç\ë¢ÇÃé¿ëï(óDêÊìx:í·)

struct Transform
{
public:
	Vector3 Position;
	Vector3 Scale = Vector3::ONE;

	//std::weak_ptr<Transform> Parent;

public:
	Vector3 GetRotation();

	Vector3 GetForward();
	Vector3 GetUp();
	Vector3 GetRight();

public:
	void RotateAxisX(float angle_);
	void RotateAxisY(float angle_);
	void RotateAxisZ(float angle_);
	void RotateAxisRight(float angle_);

	void Translate(Vector3 vec_);

#ifdef DEBUG
	void DebugDrow();

#endif

public:
	Transform(Vector3 position_);
	Transform(Vector3 position_, Vector3 scale_);

private:
	Vector3 angleRotation = Vector3::ZERO;
	Quartanion quartanion = Quartanion::IDENTITY;

	//std::vector<std::shared_ptr<Transform>> children;
};