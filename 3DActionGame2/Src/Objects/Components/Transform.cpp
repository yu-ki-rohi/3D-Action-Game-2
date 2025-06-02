#include "Transform.h"
#include <math.h>
#include <DxLib.h>

Vector3 Transform::GetRotation() const
{
	return rotateAngle;
}

Vector3 Transform::GetForward() const
{
	return quartanion.GetForward();
}

Vector3 Transform::GetUp() const
{
	return quartanion.GetUp();
}

Vector3 Transform::GetRight() const
{
	return quartanion.GetRight();
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

const Quartanion& Transform::GetQuartanion() const
{
	return quartanion;
}

void Transform::SetForward(const Vector3& forward_)
{
	float virtical_theta = asinf(forward_.y);
	float horizontal_theta = atan2f(forward_.x, forward_.z);

	quartanion =
		Quartanion::GetRotateQuartanion(horizontal_theta * 180.0f / (float)DX_PI, Vector3::UP) *
		Quartanion::GetRotateQuartanion(virtical_theta * 180.0f / (float)DX_PI, Vector3::RIGHT);

	rotateAngle = quartanion.ToEuler();
}

void Transform::SetRotate(const Vector3& rotate_)
{
	rotateAngle = rotate_;
	quartanion = Quartanion::ConvertFromEular(rotate_);
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
	quartanion = Quartanion::ConvertFrom3x3Matrix(col[0], col[1], col[2]);

}

void Transform::RotateAxisX(float angle_)
{
	if (angle_ == 0.0f) return;

	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::RIGHT);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	rotateAngle = quartanion.ToEuler();
}

void Transform::RotateAxisY(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::UP);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	rotateAngle = quartanion.ToEuler();
}

void Transform::RotateAxisZ(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, Vector3::FORWARD);
	new_quartanion *= quartanion;
	quartanion = new_quartanion;
	rotateAngle = quartanion.ToEuler();
}

void Transform::RotateAxisRight(float angle_)
{
	if (angle_ == 0.0f) return;
	Quartanion new_quartanion = Quartanion::GetRotateQuartanion(angle_, GetRight());
	new_quartanion *= quartanion;
	quartanion = new_quartanion;

	rotateAngle = quartanion.ToEuler();
}

void Transform::Translate(const Vector3& vec_)
{
	Position += quartanion.GetForward() * vec_.z;
	Position += quartanion.GetUp() * vec_.y;
	Position += quartanion.GetRight() * vec_.x;
}

Transform::Transform(Vector3 position_) :
	Position(position_)
{

}

Transform::Transform(Vector3 position_, Vector3 scale_) :
	Position(position_),
	Scale(scale_)
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