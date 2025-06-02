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

	// Boxから見た、Sphereの中心の座標を取得
	Vector3 relative_position = Vector3::BasisTransformation(sphere_collider_->GetPosition(), box_collider_->GetQuartanion(), box_collider_->GetPosition());

	// rp : relative position の頭文字
	float rp_components[Vector3::COMPONENTS_NUM];
	relative_position.ToArray(rp_components);

	float box_scale[Vector3::COMPONENTS_NUM];
	box_collider_->GetScale().ToArray(box_scale);

	// boxの頂点からsphereの中心へのベクトル
	// 中心の位置がどの象限にあるかは問題ではないので、全て第一象限にあるものとして扱える
	float vertex_to_position_vec[Vector3::COMPONENTS_NUM];

	for (int i = 0; i < Vector3::COMPONENTS_NUM; i++)
	{
		// (sphereの中心座標の絶対値) - (boxのscale) 
		vertex_to_position_vec[i] = fabsf(rp_components[i]) - box_scale[i];

		// boxの内側に向かう成分は無視
		if (vertex_to_position_vec[i] < 0.0f)
		{
			vertex_to_position_vec[i] = 0.0f;
		}
	}

	float sphere_radius = sphere_collider_->GetRadius();

	// 最近傍点から円の中心へのベクトルの長さの二乗 と 球の半径の二乗 を比較
	if (Vector3(vertex_to_position_vec[0], vertex_to_position_vec[1], vertex_to_position_vec[2]).sqrLength() < sphere_radius * sphere_radius)
	{
		return true;
	}

	return false;
}

// 現在実装中
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

	// OBBのローカル軸でSATを行う
	if (CheckOBBLoacalAxisSAT(axes, vertices_list) == true)
	{
		// 分離軸が見つかったので当たっていない
		return false;
	}

	if (CheckOBBCrossVecSAT(axes, vertices_list) == true)
	{
		// 分離軸が見つかったので当たっていない
		return false;
	}

	return true;
}


// 分離軸を見つける
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

	// 交差しているので分離軸ではない
	if (ranges[0].min <= ranges[1].max &&
		ranges[1].min <= ranges[0].max)
	{
		return false;
	}

	return true;
}

// OBBのローカル軸でSATを行う
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
				// 分離軸が見つかった
				return true;
			}
		}
	}
	return false;
}

// OBBのローカル軸同士で外積を行い、そのベクトルでSATを行う
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
				// 分離軸が見つかった
				return true;
			}
		}
	}

	return false;
}

#endif