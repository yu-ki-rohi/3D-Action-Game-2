#include "CollisionManager.h"
#include "Collider.h"
#include "../Mathmatics/Vector3.h"
#include "../Mathmatics/Quartanion.h"

void CollisionManager::CheckCollision()
{
	// 衝突確認
	// 同じ相手との重複チェックは不要
	// 最後尾は確認不要なので bodies.size() - 1
	for (int body_index = 0; body_index < bodies.size() - 1; body_index++)
	{
		// 重複チェックを防ぐため、先頭位置は body_index + 1
		for (int other_index = body_index + 1; other_index < bodies.size(); other_index++)
		{
			if (IsColliding(bodies[body_index], bodies[other_index]))
			{

			}
			else
			{

			}
		}
	}

	for (auto body : bodies)
	{
		for (auto trigger : triggers)
		{
			if (IsColliding(body, trigger))
			{

			}
			else
			{

			}
		}
	}
}

bool CollisionManager::IsColliding(const Collider* collider_01_, const Collider* collider_02_)
{
	// 球ではない場合は、Bounding Sphere で球と球の当たりを確認
	if (IsCollidingSphereAndSphere(collider_01_,collider_02_))
	{
		// ColliderのTypeの組み合わせで分岐
		// Sphere と Sphere
		if (collider_01_->GetType() == Collider::Type::Sphere && collider_02_->GetType() == Collider::Type::Sphere)
		{
			// 既に判定済み
			return true;
		}
		// Box と Box
		else if (collider_01_->GetType() == Collider::Type::Box && collider_02_->GetType() == Collider::Type::Box)
		{
			return IsCollidingBoxAndBox(collider_01_, collider_02_);
		}
		// Box と Sphere
		else
		{
			// どちらが Box でどちらが Sphere かを確認
			if (collider_01_->GetType() == Collider::Type::Box)
			{
				return IsCollidingBoxAndSphere(collider_01_, collider_02_);
			}
			else
			{
				return IsCollidingBoxAndSphere(collider_02_, collider_01_);
			}
		}
	}
	return false;
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
	// 先に SphereAndSphere の方を通っているので、ここでは判定不要だが念のため
	if (box_collider_ == nullptr || sphere_collider_ == nullptr) { return false; }

	// Boxから見た、Sphereの中心の座標を取得
	Vector3 relative_position = Vector3::BasisTransformation(sphere_collider_->GetPosition(), box_collider_->GetQuartanion(), box_collider_->GetPosition());

	// rp : relative position の頭文字
	float rp_components[Vector3::COMPONENTS_NUM];
	relative_position.ToArray(rp_components);

	float box_scale[Vector3::COMPONENTS_NUM];
	box_collider_->GetScale().ToArray(box_scale);

	// boxの頂点からsphereの中心へのベクトル
	float vertex_to_position_vec[Vector3::COMPONENTS_NUM];

	// boxからの座標に変換しているため、sphereの中心の位置がどの象限にあったとしても、
	// 符号を反転させて第一象限にあるものとして扱える
	// (任意の2軸が張る面に対して、面対称であるため)

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

bool CollisionManager::IsCollidingBoxAndBox(const Collider* collider_01_, const Collider* collider_02_)
{
	// 先に SphereAndSphere の方を通っているので、ここでは判定不要だが念のため
	if (collider_01_ == nullptr || collider_02_ == nullptr) { return false; }

	Vector3 vertices_list[2][8]{};

	auto get_vertices = [&vertices_list](const Collider* collider_, int index)
	{
		if (index < 0 || index > 2) { return; }
		vertices_list[index][0] = (collider_->GetQuartanion().GetRight() * collider_->GetScale().x + collider_->GetQuartanion().GetUp() * collider_->GetScale().y + collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 手前右上
		vertices_list[index][1] = (collider_->GetQuartanion().GetRight() * collider_->GetScale().x - collider_->GetQuartanion().GetUp() * collider_->GetScale().y + collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 手前右下
		vertices_list[index][2] = (-collider_->GetQuartanion().GetRight() * collider_->GetScale().x - collider_->GetQuartanion().GetUp() * collider_->GetScale().y + collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 手前左下
		vertices_list[index][3] = (-collider_->GetQuartanion().GetRight() * collider_->GetScale().x + collider_->GetQuartanion().GetUp() * collider_->GetScale().y + collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 手前左上
		vertices_list[index][4] = (collider_->GetQuartanion().GetRight() * collider_->GetScale().x + collider_->GetQuartanion().GetUp() * collider_->GetScale().y - collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 奥右上
		vertices_list[index][5] = (collider_->GetQuartanion().GetRight() * collider_->GetScale().x - collider_->GetQuartanion().GetUp() * collider_->GetScale().y - collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 奥右下
		vertices_list[index][6] = (-collider_->GetQuartanion().GetRight() * collider_->GetScale().x - collider_->GetQuartanion().GetUp() * collider_->GetScale().y - collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 奥左下
		vertices_list[index][7] = (-collider_->GetQuartanion().GetRight() * collider_->GetScale().x + collider_->GetQuartanion().GetUp() * collider_->GetScale().y - collider_->GetQuartanion().GetForward() * collider_->GetScale().z) * 0.5f;	// 奥左上	
	};

	get_vertices(collider_01_, 0);
	get_vertices(collider_02_, 1);




	Quartanion axes[2] = { collider_01_->GetQuartanion(), collider_02_->GetQuartanion() };

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
bool CollisionManager::IsFindOBBSparationAxis(const Vector3& axis_, Vector3 vertices_01_[8], Vector3 vertices_02_[8])
{
	Vector3* vertices_list[2]
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
			Vector3 vertex = vertices_list[i][j];
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
bool CollisionManager::CheckOBBLoacalAxisSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8])
{
	for (int i = 0; i < 2; i++)
	{
		Vector3 axes[3] = { axes_list_[i].GetRight(),axes_list_[i].GetUp(), axes_list_[i].GetForward() };

		for (int j = 0; j < 3; j++)
		{
			if (IsFindOBBSparationAxis(axes[j].Normalize(), vertices_list_[0], vertices_list_[1]) == true)
			{
				// 分離軸が見つかった
				return true;
			}
		}
	}
	return false;
}

// OBBのローカル軸同士で外積を行い、そのベクトルでSATを行う
bool CollisionManager::CheckOBBCrossVecSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8])
{
	for (int i = 0; i < 3; i++)
	{
		Vector3 collider_a_axes[3] = { axes_list_[0].GetRight(),axes_list_[0].GetUp(), axes_list_[0].GetForward() };

		for (int j = 0; j < 3; j++)
		{
			Vector3 collider_b_axes[3] = { axes_list_[1].GetRight(),axes_list_[1].GetUp(), axes_list_[1].GetForward() };
			Vector3 cross{ Vector3::Cross(collider_a_axes[i], collider_b_axes[j]) };

			if (IsFindOBBSparationAxis(cross.Normalize(), vertices_list_[0], vertices_list_[1]) == true)
			{
				// 分離軸が見つかった
				return true;
			}
		}
	}

	return false;
}