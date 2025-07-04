#pragma once
#include <vector>
#include <memory>

// 前方宣言
class Collider;
class ObjectBase;
struct Vector3;
struct Quaternion;

class CollisionManager
{
public:
	CollisionManager() = default;

public:

	void RegisterBody(std::shared_ptr<ObjectBase> owner_, Collider* collider_);
	void RegisterTrigger(std::shared_ptr<ObjectBase> owner_, Collider* collider_);

	void ReleaseBody(const Collider* collider_);
	void ReleaseTrigger(const Collider* collider_);

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
	bool CheckOBBLoacalAxisSAT(Quaternion axes_list_[2], Vector3 vertices_list_[2][8]);
	bool CheckOBBCrossVecSAT(Quaternion axes_list_[2], Vector3 vertices_list_[2][8]);

	// 前のフレームで当たっていたか
	// オプションとして、第三引数にtrueを渡すと該当のヒット記録を削除できる
	bool WasCollided(const Collider* collider_01_, const Collider* collider_02_, bool does_erase_ = false);

	// Ownerが既にいないコライダーの消去
	void EraseColliderPtrWhoseOwnerHasVanished();

	// ヒットコライダーペアの消去
	void EraseColliderPair(const Collider* collider_);


	void CheckBodyAndBody();
	void CheckBodyAndTrigger();

	// Hitした位置をコライダーに設定
	void SetHitPosition(Collider* collider_01_, Collider* collider_02_);

private:
	std::vector<std::pair<std::weak_ptr<ObjectBase>, Collider*>> bodies;
	std::vector<std::pair<std::weak_ptr<ObjectBase>, Collider*>> triggers;
	std::vector<std::pair<const Collider*, const Collider*>> preCollided;
};