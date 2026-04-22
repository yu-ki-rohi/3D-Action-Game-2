#pragma once
#include<memory>

#include "../Common.h"
#include "../DataBase/DataKind.h"

// AI関連

namespace BehaviorTree
{
	enum class Status;
	class CompositeNode;
}

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

		~EnemyBrain();

	public:
		// NOTE: 既存の枠組みに合わせるため一旦この関数でAnimatorとTransformをセットする形に
		void SetReference(std::shared_ptr<ObjectBase> owner_, std::shared_ptr<Animator> animator_, std::shared_ptr<Transform> transform_);

		void SetLocalTimeScale(float time_scale_);

	public:
		void Execute(float elapsed_time_);

#ifdef DEBUG
		void DebugDrow();
#endif

	private:
		// 実際の行動として登録する関数
		// HACK: 関数の中身の修正
		BehaviorTree::Status MoveStart(float elapsed_time_);
		BehaviorTree::Status MoveForwardStart(float elapsed_time_);
		BehaviorTree::Status MoveForward(float elapsed_time_);
		BehaviorTree::Status MoveSide(float elapsed_time_);
		BehaviorTree::Status MoveSideStart(float elapsed_time_);
		BehaviorTree::Status MoveBackward(float elapsed_time_);
		BehaviorTree::Status MoveBackwardStart(float elapsed_time_);
		BehaviorTree::Status Idle(float elapsed_time_);
		BehaviorTree::Status AttackStart0(float elapsed_time_);
		BehaviorTree::Status AttackStart1(float elapsed_time_);
		BehaviorTree::Status AttackStart2(float elapsed_time_);
		BehaviorTree::Status Attack(float elapsed_time_);


		BehaviorTree::Status AttackStart(AKind anim_kind_);

		void EnableAttackCollider();
		void DisableAttackCollider();

		bool IsNullPtrToComponent();

	private:
		// HACK: 値を適切な場所に移動
		 
		// 移動をする閾値
		static constexpr float moveStartThreshold = 20.0f;
		static constexpr float moveEndThreshold = 10.0f;
		// アニメーション変更のボーダー
		static constexpr float animationChangeBorderPercentage = 0.9f;

		static constexpr float moveForwardSpeed = 25.0f;
		static constexpr float moveSideSpeed = 60.0f;
		static constexpr float moveBackwardSpeed = 15.0f;
		static constexpr float rotateSpeed = 150.0f;

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

	};

}