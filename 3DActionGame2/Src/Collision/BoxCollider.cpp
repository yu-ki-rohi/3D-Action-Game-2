#include "BoxCollider.h"
#include "../Objects/ObjectBase.h"

#define VERTEX_NUM 8

BoxCollider::BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_) :
	Collider((localTransform.Scale * 0.5f).Length())
{
	localTransform.Position = position_;
	localTransform.Scale = scale_;

	// Eular => Quartanion の変換
	localTransform.SetRotate(rotate_ * DX_PI / 180.0f);
}

BoxCollider::BoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, std::shared_ptr<ObjectBase> owner_) :
	Collider((localTransform.Scale * 0.5f).Length(), owner_)
{
	localTransform.Position = position_;
	localTransform.Scale = scale_;

	// Eular => Quartanion の変換
	localTransform.SetRotate(rotate_ * DX_PI / 180.0f);
}

Vector3 BoxCollider::GetPosition() const
{
	return transform.Position;
}
Vector3 BoxCollider::GetScale() const
{
	return transform.Scale;
}
Vector3 BoxCollider::GetRotate() const
{
	return transform.GetRotation();
}

Quartanion BoxCollider::GetQuartanion() const
{
	return transform.GetQuartanion();
}

Collider::Type BoxCollider::GetType() const
{
	return Type::Box;
}

void BoxCollider::UpdateFromParentMat(const MATRIX& parent_mat_)
{
	MATRIX transformMat = MGetIdent();

	// ローカルのTransform情報を行列に変換
	transformMat = MMult(transformMat, localTransform.GetScaleMat());
	transformMat = MMult(transformMat, localTransform.GetRotateMat());
	transformMat = MMult(transformMat, localTransform.GetTranslateMat());
	// 親情報の反映
	transformMat = MMult(transformMat, parent_mat_);

	transform.UpdateFromMatrix(transformMat);

}

void BoxCollider::UpdatePosition(const Vector3& new_position_)
{
	transform.Position = new_position_;
}

void BoxCollider::UpdateRadius()
{
	radius = (transform.Scale * 0.5f).Length();
}

#ifdef DEBUG
void BoxCollider::DebugDrow()
{
	// 0:手前右上
	// 1:手前右下
	// 2:手前左下
	// 3:手前左上
	// 4:  奥右上
	// 5:  奥右下
	// 6:  奥左下
	// 7:  奥左上
	Vector3 vertices[VERTEX_NUM] = {
		( transform.GetRight() * transform.Scale.x + transform.GetUp() * transform.Scale.y + transform.GetForward() * transform.Scale.z) * 0.5f,	// 手前右上
		( transform.GetRight() * transform.Scale.x - transform.GetUp() * transform.Scale.y + transform.GetForward() * transform.Scale.z) * 0.5f,	// 手前右下
		(-transform.GetRight() * transform.Scale.x - transform.GetUp() * transform.Scale.y + transform.GetForward() * transform.Scale.z) * 0.5f,	// 手前左下
		(-transform.GetRight() * transform.Scale.x + transform.GetUp() * transform.Scale.y + transform.GetForward() * transform.Scale.z) * 0.5f,	// 手前左上
		( transform.GetRight() * transform.Scale.x + transform.GetUp() * transform.Scale.y - transform.GetForward() * transform.Scale.z) * 0.5f,	// 奥右上
		( transform.GetRight() * transform.Scale.x - transform.GetUp() * transform.Scale.y - transform.GetForward() * transform.Scale.z) * 0.5f,	// 奥右下
		(-transform.GetRight() * transform.Scale.x - transform.GetUp() * transform.Scale.y - transform.GetForward() * transform.Scale.z) * 0.5f,	// 奥左下
		(-transform.GetRight() * transform.Scale.x + transform.GetUp() * transform.Scale.y - transform.GetForward() * transform.Scale.z) * 0.5f 	// 奥左上
	};

	for (auto& vertice : vertices)
	{
		vertice += transform.Position;
	}

	std::vector<std::pair<Vector3, Vector3>> line_list =
	{
		{ vertices[0], vertices[1] },
		{ vertices[1], vertices[2] },
		{ vertices[2], vertices[3] },
		{ vertices[3], vertices[0] },

		{ vertices[4], vertices[5] },
		{ vertices[5], vertices[6] },
		{ vertices[6], vertices[7] },
		{ vertices[7], vertices[4] },

		{ vertices[0], vertices[4] },
		{ vertices[1], vertices[5] },
		{ vertices[2], vertices[6] },
		{ vertices[3], vertices[7] },
	};

	unsigned int color;
	if (IsEnabled())
	{
		color = GetColor(255, 255, 255);
	}
	else
	{
		color = GetColor(155, 155, 155);
	}

	for (const auto& pair : line_list)
	{
		DrawLine3D(pair.first.ToVECTOR(), pair.second.ToVECTOR(), color);
	}

	transform.DebugDrow();
}

#endif