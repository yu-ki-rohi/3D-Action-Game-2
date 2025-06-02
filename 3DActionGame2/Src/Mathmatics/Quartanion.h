#pragma once
// �Q�l�ɂ����T�C�g���ȉ��ɂ܂Ƃ߂Ă���܂�
// https://docs.google.com/document/d/1MiHupls7hHg30EK4eU9b8I1DqDNullxO5BEPAq3sJFA/edit?tab=t.0#heading=h.53o8674ws84b

struct Vector3;

struct Quartanion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	// �P�ʌ� (�܂�N�H�[�^�j�I���ɂ����āA�����ɂ�����1�ɑ����������)
	static const Quartanion IDENTITY;

public:
	// �C�ӎ��ŉ�]������N�H�[�^�j�I����Ԃ�
	// �p�x�̒P�ʂ͓x��
	static Quartanion GetRotateQuartanion(float degree_, Vector3 axis_);

	// �I�C���[�p����N�H�[�^�j�I���ւ̕ϊ�
	// ��]����XYZ
	static Quartanion ConvertFromEular(const Vector3& eular_);

	// ��]�s�񂩂�N�H�[�^�j�I���ւ̕ϊ�
	// ��]�s��͗�x�N�g����3���ׂ����̂Ƃ��Ĉ���
	static Quartanion ConvertFrom3x3Matrix(const Vector3& col_00_, const Vector3& col_01_, const Vector3& col_02_);

public:
	Quartanion(float x_, float y_, float z_, float w_);

public:
	// �N�H�[�^�j�I�������]�s��̈ꕔ�֕ϊ�
	// ���ꂼ��O�A��A�E�����̃x�N�g����Ԃ�
	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetRight() const;

public:
	// �I�C���[�p�ւ̕ϊ�
	Vector3 ToEuler() const;

public:
	Quartanion operator*(const Quartanion& other);
	Quartanion& operator*=(const Quartanion& other);
};