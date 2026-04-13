#pragma once
#include<memory>
#include "BehaviorTree/Node.h"

// todo: 前方宣言で出来ないかどうか調べる
#include "BehaviorTree/CompositeNode.h"

#include "../Common.h"

// AI関連

// Component関連
class Animator;
class Transform;

class Collider;
class TimerBase;

class ObjectBase;

// 各エネミーの行動決定
namespace EnemyAI
{
	class IEnemyDirectiveReader;

	class EnemyBrain
	{
	public:
		// HACK: 引数部分を構造体化するなどの工夫
		EnemyBrain(
			int id_,
			std::shared_ptr<IEnemyDirectiveReader> directive_,
			std::shared_ptr<Collider> attackCollider_,
			std::shared_ptr<Collider> justAvoidIgnitionCollider_);

	public:
		// NOTE: 既存の枠組みに合わせるため一旦この関数でAnimatorとTransformをセットする形に
		void SetReference(std::shared_ptr<ObjectBase> owner_, std::shared_ptr<Animator> animator_, std::shared_ptr<Transform> transform_);

	public:
		void Execute(float elapsed_time_);

#ifdef DEBUG
		void DebugDrow();
#endif

	private:
		// 実際の行動として登録する関数
		// HACK: 関数の中身の修正
		BehaviorTree::Status MoveStart(float elapsed_time_);
		BehaviorTree::Status Move(float elapsed_time_);
		BehaviorTree::Status Idle(float elapsed_time_);
		BehaviorTree::Status AttackStart(float elapsed_time_);
		BehaviorTree::Status Attack(float elapsed_time_);

		void EnableAttackCollider();
		void DisableAttackCollider();

	private:
		// HACK: 値を適切な場所に移動
		 
		// 移動をする閾値
		static constexpr float moveThreshold = 0.25f;
		// アニメーション変更のボーダー
		static constexpr float animationChangeBorderPercentage = 0.9f;

		static constexpr float moveSpeed = 30.0f;

	private:
		int id;

		// AI関連
		std::shared_ptr<IEnemyDirectiveReader> directive;

		// 検討事項: BehaviorTreeを個別に持たせるかどうか
		std::unique_ptr<BehaviorTree::CompositeNode> behaviorTree;

		std::weak_ptr<ObjectBase> owner;

		// Component関連
		std::shared_ptr<Animator> animator;
		std::shared_ptr<Transform> transform;


		std::shared_ptr<Collider> attackCollider;
		std::shared_ptr<Collider> justAvoidIgnitionCollider;

		std::shared_ptr<TimerBase> enableColliderTimer;
		std::shared_ptr<TimerBase> disableColliderTimer;


		// NOTE: アニメーション切替の挙動を連続で行わせないため　けっこう苦し紛れなので変えたい
		// HACK: ここの見直し
		bool isChangeingAttackAnimation;

	};

}