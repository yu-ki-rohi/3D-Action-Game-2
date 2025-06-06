#pragma once
#include <vector>
#include <memory>

class ColliderRegisterInterface;

// 前方宣言
class Collider;
struct Vector3;
struct Quartanion;

/*
	ダメージ処理実装案
	A.
	GetComponentを用意してタグ付けして関数実行
	利点
	汎用性が高い
	Unityライクのコンセプトに沿う
	問題点
	dynamic_castの発生による安全性・処理時間等の問題

	B.
	CharacterBaseに派生させる
	利点
	C++
	問題点
	静的オブジェクトと共通化できなくなる

	C.
	DataManager的なクラスを用意しそちら経由で情報を取得
	利点

	問題点


*/


class CollisionManager : public std::enable_shared_from_this<CollisionManager>
{
public:
	// 基本的にこれで取得したポインタを保持し続ける場合はweak_ptrにする
	std::shared_ptr<ColliderRegisterInterface> GetRegisterInterface();

	void CheckCollision();

private:
	bool IsColliding(const Collider* collider_01_, const Collider* collider_02_);

	bool IsCollidingSphereAndSphere(const Collider* collider_01_, const Collider* collider_02_);
	bool IsCollidingBoxAndSphere(const Collider* box_collider_, const Collider* sphere_collider_);

	// Box(Obb) と Box(Obb) が当たっているかの確認
	// 多少アレンジをしてはいるものの、ほぼ先生から頂いたソースを使用しています
	bool IsCollidingBoxAndBox(const Collider* collider_01_, const Collider* collider_02_);

	// IsCollidingBoxAndBoxの中で呼び出される関数
	bool IsFindOBBSparationAxis(const Vector3& axis_, Vector3 vertices_01_[8], Vector3 vertices_02_[8]);
	bool CheckOBBLoacalAxisSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8]);
	bool CheckOBBCrossVecSAT(Quartanion axes_list_[2], Vector3 vertices_list_[2][8]);

private:
	std::shared_ptr<ColliderRegisterInterface> colliderRegisterInterface;

	std::vector<Collider*> bodies;
	std::vector<Collider*> triggers;
};