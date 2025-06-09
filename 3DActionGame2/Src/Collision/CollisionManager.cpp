#include "CollisionManager.h"
#include "Collider.h"
#include "ColliderRegisterInterface.h"
#include "../Mathmatics/Vector3.h"
#include "../Mathmatics/Quartanion.h"

void CollisionManager::RegisterBody(std::weak_ptr<ObjectBase> owner_, const Collider* collider_)
{
	bodies.push_back(std::make_pair(owner_, collider_));
}

void CollisionManager::RegisterTrigger(std::weak_ptr<ObjectBase> owner_, const Collider* collider_)
{
	triggers.push_back(std::make_pair(owner_, collider_));
}

void CollisionManager::ReleaseBody(const Collider* collider_)
{
	for (auto itr = bodies.begin(); itr != bodies.end(); )
	{
		if ((*itr).second == collider_)
		{
			itr = bodies.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void CollisionManager::ReleaseTrigger(const Collider* collider_)
{
	for (auto itr = triggers.begin(); itr != triggers.end(); )
	{
		if ((*itr).second == collider_)
		{
			itr = triggers.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void CollisionManager::CheckCollision()
{
	// コライダーの親が存在しない場合の消去作業
	EraseColliderPtrWhoseOwnerHasVanished();

	// 衝突確認
	CheckBodyAndBody();

	CheckBodyAndTrigger();
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

bool CollisionManager::WasCollided(const Collider* collider_01_, const Collider* collider_02_, bool does_erase_)
{
	for (auto itr = preCollided.begin(); itr != preCollided.end(); )
	{
		if ((*itr).first == collider_01_ && (*itr).second == collider_02_)
		{
			if (does_erase_)
			{
				itr = preCollided.erase(itr);
			}
			return true;
		}
	}
	return false;
}

void CollisionManager::EraseColliderPtrWhoseOwnerHasVanished()
{
	std::vector<const Collider*> removed_colliders;
	for (auto body : bodies)
	{
		auto obj = body.first.lock();
		if (obj == nullptr)
		{
			removed_colliders.push_back(body.second);
		}
	}
	for (auto removed_collider : removed_colliders)
	{
		ReleaseBody(removed_collider);
		EraseColliderPair(removed_collider);
	}
	removed_colliders.clear();

	for (auto trigger : triggers)
	{
		auto obj = trigger.first.lock();
		if (obj == nullptr)
		{
			removed_colliders.push_back(trigger.second);
		}
	}
	for (auto removed_collider : removed_colliders)
	{
		ReleaseBody(removed_collider);
		EraseColliderPair(removed_collider);
	}
}
void CollisionManager::EraseColliderPair(const Collider* collider_)
{
	for (auto itr = preCollided.begin(); itr != preCollided.end(); )
	{
		if ((*itr).first == collider_ || (*itr).second == collider_)
		{
			itr = preCollided.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void CollisionManager::CheckBodyAndBody()
{
	// 同じ相手との重複チェックは不要
	// 最後尾は確認不要なので bodies.size() - 1
	for (int body_index = 0; body_index < bodies.size() - 1; body_index++)
	{
		// 重複チェックを防ぐため、先頭位置は body_index + 1
		for (int other_index = body_index + 1; other_index < bodies.size(); other_index++)
		{
			// ヒット確認
			if (IsColliding(bodies[body_index].second, bodies[other_index].second))
			{
				// ヒット時
				// 前フレームで当たっていたかを確認
				if (WasCollided(bodies[body_index].second, bodies[other_index].second))
				{
					// 当たり継続
					bodies[body_index].second->OnCollisionStay(bodies[other_index].second);
					bodies[other_index].second->OnCollisionStay(bodies[body_index].second);
				}
				else
				{
					// 当たった瞬間
					bodies[body_index].second->OnCollisionEnter(bodies[other_index].second);
					bodies[other_index].second->OnCollisionEnter(bodies[body_index].second);
					preCollided.push_back(std::make_pair(bodies[body_index].second, bodies[other_index].second));
				}
			}
			else
			{
				// 当たっていない場合
				// 前フレームまで当たっていたかを確認
				// 当たっていた場合、記録の除去も同時に実行
				if (WasCollided(bodies[body_index].second, bodies[other_index].second, true))
				{
					bodies[body_index].second->OnCollisionExit(bodies[other_index].second);
					bodies[other_index].second->OnCollisionExit(bodies[body_index].second);
				}
			}
		}
	}
}

void CollisionManager::CheckBodyAndTrigger()
{
	for (auto body : bodies)
	{
		for (auto trigger : triggers)
		{
			if (IsColliding(body.second, trigger.second))
			{
				// ヒット時
				// 前フレームで当たっていたかを確認
				if (WasCollided(body.second, trigger.second))
				{
					// 当たり継続
					trigger.second->OnTriggerStay(body.second);
				}
				else
				{
					// 当たった瞬間
					trigger.second->OnCollisionEnter(body.second);
					preCollided.push_back(std::make_pair(body.second, trigger.second));
				}
			}
			else
			{
				// 当たっていない場合
				// 前フレームまで当たっていたかを確認
				// 当たっていた場合、記録の除去も同時に実行
				if (WasCollided(body.second, trigger.second, true))
				{
					trigger.second->OnTriggerExit(body.second);
				}
			}
		}
	}
}