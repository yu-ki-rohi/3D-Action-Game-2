#include "Transform.h"
#include <math.h>
#include <DxLib.h>

Vector3 Transform::GetRotation() const
{
	return rotateAngle;
}

Vector3 Transform::GetForward() const
{
	return quaternion.GetForward();
}

Vector3 Transform::GetUp() const
{
	return quaternion.GetUp();
}

Vector3 Transform::GetRight() const
{
	return quaternion.GetRight();
}

MATRIX Transform::GetTranslateMat() const
{
	MATRIX translateMat = MGetIdent();
	CreateTranslationMatrix(&translateMat, Position.x, Position.y, Position.z);
	return translateMat;
}

MATRIX Transform::GetRotateMat() const
{
	MATRIX rotateMat = MGetIdent();
	CreateRotationXYZMatrix(&rotateMat, rotateAngle.x, rotateAngle.y, rotateAngle.z);
	return rotateMat;
}

MATRIX Transform::GetScaleMat() const
{
	MATRIX scaleMat = MGetIdent();
	CreateScalingMatrix(&scaleMat, Scale.x, Scale.y, Scale.z);
	return scaleMat;
}

const Quaternion& Transform::GetQuaternion() const
{
	return quaternion;
}

void Transform::SetForward(const Vector3& forward_)
{
	StartSlearpByForwardAndDuration(forward_, 0.0f);/*
	quaternion = Quaternion::GetQuaternionByForward(forward_);
	rotateAngle = quaternion.ToEuler();
	isSlearp = false;*/
}

void Transform::SetRotate(const Vector3& rotate_)
{
	rotateAngle = rotate_;
	quaternion = Quaternion::ConvertFromEular(rotate_);
	isSlearp = false;
}

void Transform::SetQuaternion(const Quaternion& quaternion_)
{
	quaternion = quaternion_;
	rotateAngle = quaternion.ToEuler();
	isSlearp = false;
}

void Transform::StartSlearpByForwardAndDuration(const Vector3& target_forward_, float duration_)
{
	slearpTarget = Quaternion::GetQuaternionByForward(target_forward_);
	slearpDuration = duration_;

	slearpInit = quaternion;
	slearpTime = 0.0f;

	isSlearp = true;
}

void Transform::StartSlearpByForwardAndAngularVelocity(const Vector3& target_forward_, float angular_velocity_)
{
	// äpë¨ìx0Ç»ÇÁÇŒâÒì]ÇµÇ»Ç¢
	if (angular_velocity_ == 0.0f) { return; }
	float dot = Vector3::Dot(target_forward_.Normalize(), quaternion.GetForward());
	float radian = acosf(dot);

	float duration = radian * 180.0f / DX_PI / angular_velocity_;
	
	StartSlearpByForwardAndDuration(target_forward_, duration);
}

void Transform::UpdateFromMatrix(const MATRIX& transform_mat_)
{
	// ç¿ïWë„ì¸
	Position.x = transform_mat_.m[3][0];
	Position.y = transform_mat_.m[3][1];
	Position.z = transform_mat_.m[3][2];

	// ägèkë„ì¸
	const int col_num = 3;
	Vector3 col[col_num] = {};
	float* scale[col_num] = { &Scale.x, &Scale.y, &Scale.z };
	for (int i = 0; i < col_num; i++)
	{
		col[i] = Vector3(transform_mat_.m[i][0], transform_mat_.m[i][1], transform_mat_.m[i][2]);
		*scale[i] = col[i].Length();

		// âÒì]ÇÃÇΩÇﬂÇÃèÄîı
		if (*scale[i] == 0) { continue; }
		float reciprocal = 1.0f / *scale[i];
		col[i] *= reciprocal;
	}

	// âÒì]ë„ì¸
	// colÇï¿Ç◊ÇΩÇ‡ÇÃÇ™âÒì]çsóÒÇ»ÇÃÇ≈ÅAÇ±Ç±Ç≈ÇÕÉNÉHÅ[É^ÉjÉIÉìÇ÷ÇÃïœä∑ÇÇ∑ÇÈÅB
	quaternion = Quaternion::ConvertFrom3x3Matrix(col[0], col[1], col[2]);

}

void Transform::Update(float elapsed_time_)
{
	if (isSlearp)
	{
		SlearpRotation(elapsed_time_);
	}

	Position += quaternion.GetForward() * velocity.z * elapsed_time_;
	Position += quaternion.GetUp() * velocity.y * elapsed_time_;
	Position += quaternion.GetRight() * velocity.x * elapsed_time_;
}

void Transform::SlearpRotation(float elapsed_time_)
{
	if (slearpTime < fabsf(slearpDuration))
	{
		slearpTime += elapsed_time_;
		quaternion = Quaternion::Slearp(slearpInit, slearpTarget, slearpTime / slearpDuration);
		rotateAngle = quaternion.ToEuler();
	}
	else
	{
		quaternion = slearpTarget;
		rotateAngle = quaternion.ToEuler();
		isSlearp = false;
	}
}

void Transform::RotateAxisX(float angle_)
{
	if (angle_ == 0.0f) return;

	Quaternion new_quaternion = Quaternion::GetRotateQuaternion(angle_, Vector3::RIGHT);
	new_quaternion *= quaternion;
	quaternion = new_quaternion;
	rotateAngle = quaternion.ToEuler();
}

void Transform::RotateAxisY(float angle_)
{
	if (angle_ == 0.0f) return;
	Quaternion new_quaternion = Quaternion::GetRotateQuaternion(angle_, Vector3::UP);
	new_quaternion *= quaternion;
	quaternion = new_quaternion;
	rotateAngle = quaternion.ToEuler();
}

void Transform::RotateAxisZ(float angle_)
{
	if (angle_ == 0.0f) return;
	Quaternion new_quaternion = Quaternion::GetRotateQuaternion(angle_, Vector3::FORWARD);
	new_quaternion *= quaternion;
	quaternion = new_quaternion;
	rotateAngle = quaternion.ToEuler();
}

void Transform::RotateAxisRight(float angle_)
{
	if (angle_ == 0.0f) return;
	Quaternion new_quaternion = Quaternion::GetRotateQuaternion(angle_, GetRight());
	new_quaternion *= quaternion;
	quaternion = new_quaternion;

	rotateAngle = quaternion.ToEuler();
}

void Transform::Translate(const Vector3& vec_)
{
	velocity = vec_;
	velocity.z *= -1;
}
Transform::Transform() :
	Position(Vector3::ZERO),
	rotateAngle(Vector3::ZERO),
	quaternion(Quaternion::IDENTITY)
{

}
Transform::Transform(Vector3 position_) :
	Position(position_),
	rotateAngle(Vector3::ZERO),
	quaternion(Quaternion::IDENTITY)
{

}

Transform::Transform(Vector3 position_, Vector3 scale_) :
	Position(position_),
	Scale(scale_),
	rotateAngle(Vector3::ZERO),
	quaternion(Quaternion::IDENTITY)
{

}

Transform::Transform(Vector3 position_, Vector3 scale_, Vector3 rotation_) :
	Position(position_),
	Scale(scale_),
	rotateAngle(rotation_),
	quaternion(Quaternion::ConvertFromEular(rotation_))
{

}

#ifdef DEBUG
void Transform::DebugDrow()
{
	VECTOR position = Position.ToVECTOR();
	float length = 20.0f;
	DrawLine3D(position, (Position + GetRight() * length).ToVECTOR(), GetColor(255, 0, 0));
	DrawLine3D(position, (Position + GetUp() * length).ToVECTOR(), GetColor(0, 255, 0));
	DrawLine3D(position, (Position + GetForward() * length).ToVECTOR(), GetColor(0, 0, 255));
}

#endif