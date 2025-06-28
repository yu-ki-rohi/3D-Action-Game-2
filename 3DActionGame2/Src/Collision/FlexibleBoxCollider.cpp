#include "FlexibleBoxCollider.h"

FlexibleBoxCollider::FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_) :
	BoxCollider(position_, scale_, rotate_)
{

}

FlexibleBoxCollider::FlexibleBoxCollider(Vector3 position_, Vector3 scale_, Vector3 rotate_, bool is_collide_once_) :
	BoxCollider(position_, scale_, rotate_, is_collide_once_)
{

}

void FlexibleBoxCollider::UpdateFromParentMat(const MATRIX& parent_mat_)
{
	// 更新前のboxTransformを保存
	Transform past_box = boxTransform;

	// boxTransformの更新
	MATRIX transformMat = MGetIdent();

	// ローカルのTransform情報を行列に変換
	transformMat = MMult(transformMat, localTransform.GetScaleMat());
	transformMat = MMult(transformMat, localTransform.GetRotateMat());
	transformMat = MMult(transformMat, localTransform.GetTranslateMat());
	// 親情報の反映
	transformMat = MMult(transformMat, parent_mat_);

	boxTransform.UpdateFromMatrix(transformMat);

	// 姿勢と座標は現在と前の中間をとる
	Quaternion new_quaternion = Quaternion::Slearp(past_box.GetQuaternion(), boxTransform.GetQuaternion(), 0.5f);
	transform.SetQuaternion(new_quaternion);

	Vector3 new_position = (past_box.Position + boxTransform.Position) * 0.5f;
	transform.Position = new_position;

	const int list_num = 2;
	const int vertex_num = 8;

	// Scaleは各頂点を含む大きさにする
	Vector3 vertices_list[list_num][vertex_num]{};

	// 頂点取得
	auto get_vertices = [&vertices_list](const Transform& transform_, int index)
	{
		if (index < 0 || index > list_num) { return; }
		vertices_list[index][0] = (-transform_.GetQuaternion().GetRight() * transform_.Scale.x + transform_.GetQuaternion().GetUp() * transform_.Scale.y + transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	// 手前左上
		vertices_list[index][1] = (transform_.GetQuaternion().GetRight() * transform_.Scale.x + transform_.GetQuaternion().GetUp() * transform_.Scale.y + transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	// 手前右上
		vertices_list[index][2] = (transform_.GetQuaternion().GetRight() * transform_.Scale.x - transform_.GetQuaternion().GetUp() * transform_.Scale.y + transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	// 手前右下
		vertices_list[index][3] = (-transform_.GetQuaternion().GetRight() * transform_.Scale.x - transform_.GetQuaternion().GetUp() * transform_.Scale.y + transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	// 手前左下
		vertices_list[index][4] = (-transform_.GetQuaternion().GetRight() * transform_.Scale.x + transform_.GetQuaternion().GetUp() * transform_.Scale.y - transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	//  奥 左上	
		vertices_list[index][5] = (transform_.GetQuaternion().GetRight() * transform_.Scale.x + transform_.GetQuaternion().GetUp() * transform_.Scale.y - transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	//  奥 右上
		vertices_list[index][6] = (transform_.GetQuaternion().GetRight() * transform_.Scale.x - transform_.GetQuaternion().GetUp() * transform_.Scale.y - transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	//  奥 右下
		vertices_list[index][7] = (-transform_.GetQuaternion().GetRight() * transform_.Scale.x - transform_.GetQuaternion().GetUp() * transform_.Scale.y - transform_.GetQuaternion().GetForward() * transform_.Scale.z) + transform_.Position;	//  奥 左下
	};

	get_vertices(past_box, 0);
	get_vertices(boxTransform, 1);

	float x = 0.0f, y = 0.0f, z = 0.0f;
	// 頂点の座標系の変換と各軸の値の比較・保存
	for (int li = 0; li < list_num; li++)
	{
		for (int vi = 0; vi < vertex_num; vi++)
		{
			vertices_list[li][vi] = Vector3::BasisTransformation(vertices_list[li][vi], new_quaternion, new_position);
			if (x < fabsf(vertices_list[li][vi].x)) 
			{
				x = fabsf(vertices_list[li][vi].x);
			}
			if (y < fabsf(vertices_list[li][vi].y)) 
			{
				y = fabsf(vertices_list[li][vi].y);
			}
			if (z < fabsf(vertices_list[li][vi].z)) 
			{
				z = fabsf(vertices_list[li][vi].z);
			}
		}
	}
	// Scaleの更新
	transform.Scale = Vector3(x, y, z);

	// Scaleが更新されたので半径の取り直し
	UpdateRadius();
}
