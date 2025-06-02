#include "CollisionManager.h"
#include "Collider.h"
#include "../Mathmatics/Vector3.h"
#include "../Mathmatics/Quartanion.h"

void CollisionManager::CheckCollision()
{

}

bool CollisionManager::IsCollidingSphereAndSphere(const Collider* collider_01_, const Collider* collider_02_)
{
	if (collider_01_ == nullptr || collider_02_ == nullptr) { return false; }

	float radius_sum = collider_01_->GetRadius() + collider_02_->GetRadius();

	if ((collider_01_->GetPosition() - collider_01_->GetPosition()).sqrLength() < radius_sum * radius_sum)
	{
		return true;
	}
	return false;
}

bool CollisionManager::IsCollidingBoxAndSphere(const Collider* box_collider_, const Collider* sphere_collider_)
{
	if (box_collider_ == nullptr || sphere_collider_ == nullptr) { return false; }

	// Box���猩���ASphere�̒��S�̍��W���擾
	Vector3 relative_position = Vector3::BasisTransformation(sphere_collider_->GetPosition(), box_collider_->GetQuartanion(), box_collider_->GetPosition());

	// rp : relative position �̓�����
	float rp_components[Vector3::COMPONENTS_NUM];
	relative_position.ToArray(rp_components);

	float box_scale[Vector3::COMPONENTS_NUM];
	box_collider_->GetScale().ToArray(box_scale);

	// box�̒��_����sphere�̒��S�ւ̃x�N�g��
	// ���S�̈ʒu���ǂ̏ی��ɂ��邩�͖��ł͂Ȃ��̂ŁA�S�đ��ی��ɂ�����̂Ƃ��Ĉ�����
	float vertex_to_position_vec[Vector3::COMPONENTS_NUM];

	for (int i = 0; i < Vector3::COMPONENTS_NUM; i++)
	{
		// (sphere�̒��S���W�̐�Βl) - (box��scale) 
		vertex_to_position_vec[i] = fabsf(rp_components[i]) - box_scale[i];

		// box�̓����Ɍ����������͖���
		if (vertex_to_position_vec[i] < 0.0f)
		{
			vertex_to_position_vec[i] = 0.0f;
		}
	}

	float sphere_radius = sphere_collider_->GetRadius();

	// �ŋߖT�_����~�̒��S�ւ̃x�N�g���̒����̓�� �� ���̔��a�̓�� ���r
	if (Vector3(vertex_to_position_vec[0], vertex_to_position_vec[1], vertex_to_position_vec[2]).sqrLength() < sphere_radius * sphere_radius)
	{
		return true;
	}

	return false;
}

// ���ݎ�����
#if false

bool CollisionManager::IsCollidingBoxAndBox(const Collider* collider_01_, const Collider* collider_02_)
{
	if (collider_01_ == nullptr || collider_02_ == nullptr) { return false; }

	VECTOR vertices_list[2][8]{};
	for (int i = 0; i < 8; i++)
	{
		vertices_list[0][i] = collider_01_.currentVertices[i];
		vertices_list[1][i] = collider_02_.currentVertices[i];
	}

	Axis axes[2]{};
	collider_01_.GetAxis(axes[0]);
	collider_02_.GetAxis(axes[1]);

	// OBB�̃��[�J������SAT���s��
	if (CheckOBBLoacalAxisSAT(axes, vertices_list) == true)
	{
		// �����������������̂œ������Ă��Ȃ�
		return false;
	}

	if (CheckOBBCrossVecSAT(axes, vertices_list) == true)
	{
		// �����������������̂œ������Ă��Ȃ�
		return false;
	}

	return true;
}


// ��������������
bool CollisionManager::IsFindOBBSparationAxis(const VECTOR& axis_, VECTOR vertices_01_[8], VECTOR vertices_02_[8])
{
	VECTOR* vertices_list[2]
	{
		vertices_01_,
		vertices_02_
	};

	struct Range
	{
		float min{ 1000000.0f };
		float max{ -1000000.0f };
	};

	Range ranges[2];

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			VECTOR vertex = vertices_list[i][j];
			float dot = axis_.x * vertex.x + axis_.y * vertex.y + axis_.z * vertex.z;

			if (ranges[i].min > dot)
			{
				ranges[i].min = dot;
			}
			if (ranges[i].max < dot)
			{
				ranges[i].max = dot;
			}
		}
	}

	// �������Ă���̂ŕ������ł͂Ȃ�
	if (ranges[0].min <= ranges[1].max &&
		ranges[1].min <= ranges[0].max)
	{
		return false;
	}

	return true;
}

// OBB�̃��[�J������SAT���s��
bool CollisionManager::CheckOBBLoacalAxisSAT(Axis axes_list_[2], VECTOR vertices_list_[2][8])
{
	for (int i = 0; i < 2; i++)
	{
		VECTOR axes[3];
		axes_list_[i].ToArray(axes);

		for (int j = 0; j < 3; j++)
		{
			if (IsFindOBBSparationAxis(VNorm(axes[j]), vertices_list_[0], vertices_list_[1]) == true)
			{
				// ����������������
				return true;
			}
		}
	}
	return false;
}

// OBB�̃��[�J�������m�ŊO�ς��s���A���̃x�N�g����SAT���s��
bool CollisionManager::CheckOBBCrossVecSAT(Axis axes_list_[2], VECTOR vertices_list_[2][8])
{
	for (int i = 0; i < 3; i++)
	{
		VECTOR collider_a_axes[3]{};
		axes_[0].ToArray(collider_a_axes);

		for (int j = 0; j < 3; j++)
		{
			VECTOR collider_b_axes[3]{};
			axes_[1].ToArray(collider_b_axes);
			VECTOR cross{ VCross(collider_a_axes[i], collider_b_axes[j]) };

			if (IsFindOBBSparationAxis(VNorm(cross), vertices_list_[0], vertices_list_[1]) == true)
			{
				// ����������������
				return true;
			}
		}
	}

	return false;
}

#endif