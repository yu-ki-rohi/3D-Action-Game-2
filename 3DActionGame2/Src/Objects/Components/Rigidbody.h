#pragma once
#include "Transform.h"
#include "ComponentBase.h"

// �֋X�� Rigid body (����) �Ƃ������O��t���Ă܂����A
// �����I�ɂ� Mass point (���_) �ł��B
// ����A�������Z�ŉ�]������\�肪�Ȃ��̂�
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
	// ���݉�����Ă����
	Vector3 force = Vector3::ZERO;
};