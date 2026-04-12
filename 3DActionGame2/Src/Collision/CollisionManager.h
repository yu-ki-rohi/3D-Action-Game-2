#pragma once
#include <vector>
#include <memory>
#include "IColliderRegister.h"

// 前方宣言
class Collider;
class ObjectBase;
struct Vector3;
struct Quaternion;

// 衝突を管理するクラス

class CollisionManager : public IColliderRegister
{
public:
	CollisionManager() = default;

public:

	// 原則コライダーの登録・登録解除はColliderRegisterInterfaceを通して行う

	void RegisterBody(std::shared_ptr<Collider> collider_) override;
	void RegisterTrigger(std::shared_ptr<Collider> collider_) override;

	void ReleaseBody( std::shared_ptr<Collider> collider_) override;
	void ReleaseTrigger( std::shared_ptr<Collider> collider_) override;


	// 当たりの確認・当たり時の処理を実行
	void CheckCollision();

	

private:
	bool IsColliding( std::shared_ptr<Collider> collider_01_,  std::shared_ptr<Collider> collider_02_);

	bool IsCollidingSphereAndSphere( std::shared_ptr<Collider> collider_01_,  std::shared_ptr<Collider> collider_02_);
	bool IsCollidingBoxAndSphere( std::shared_ptr<Collider> box_collider_,  std::shared_ptr<Collider> sphere_collider_);

	// Box(Obb) と Box(Obb) が当たっているかの確認
	// 多少アレンジをしてはいるものの、ほぼ先生から頂いたソースを使用しています
	bool IsCollidingBoxAndBox( std::shared_ptr<Collider> collider_01_,  std::shared_ptr<Collider> collider_02_);

	// IsCollidingBoxAndBoxの中で呼び出される関数
	bool IsFindOBBSparationAxis(const Vector3& axis_, Vector3 vertices_01_[8], Vector3 vertices_02_[8]);
	bool CheckOBBLoacalAxisSAT(Quaternion axes_list_[2], Vector3 vertices_list_[2][8]);
	bool CheckOBBCrossVecSAT(Quaternion axes_list_[2], Vector3 vertices_list_[2][8]);

	// 前のフレームで当たっていたか
	// オプションとして、第三引数にtrueを渡すと該当のヒット記録を削除できる
	bool WasCollided( std::shared_ptr<Collider> collider_01_,  std::shared_ptr<Collider> collider_02_, bool does_erase_ = false);

	// Ownerが既にいないコライダーの消去
	void EraseColliderPtrWhoseOwnerHasVanished();

	// ヒットコライダーペアの消去
	void EraseColliderPair( std::shared_ptr<Collider> collider_);

	// 衝突確認
	void CheckBodyAndBody();
	// トリガー確認
	void CheckBodyAndTrigger();

	// Hitした位置をコライダーに設定
	void SetHitPosition(std::shared_ptr<Collider> collider_01_, std::shared_ptr<Collider> collider_02_);

private:
	std::vector<std::shared_ptr<Collider>> bodies;
	std::vector<std::shared_ptr<Collider>> triggers;
	std::vector<std::pair< std::shared_ptr<Collider>,  std::shared_ptr<Collider>>> preCollided;
};