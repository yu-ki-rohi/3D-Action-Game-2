#include "EnemyBrain.h"
#include "IEnemyDirectiveReader.h"
#include "EnemyReport.h"
#include "EnemyReportHandler.h"

#include "../Objects/Components/Transform.h"
#include "../Objects/Components/Animator.h"
#include "../Collision/Collider.h"

#include "BehaviorTree/SelectorNode.h"
#include "BehaviorTree/SequenceNode.h"
#include "BehaviorTree/RandomNode.h"
#include "BehaviorTree/LeafNode.h"

#include "../Systems/TimerFactory.h"
#include "DxLib.h"
#include <math.h>
#include "../Debug/DebugMessenger.h"


using namespace BehaviorTree;
using namespace EnemyAI;
EnemyBrain::EnemyBrain(
	int id_,
	std::shared_ptr<EnemyReportHandler> report_handler_,
	std::shared_ptr<IEnemyDirectiveReader> directive_,
	std::shared_ptr<Collider> attackCollider_,
	std::shared_ptr<Collider> justAvoidIgnitionCollider_) :
	id(id_),
	directive(directive_),
	report(std::make_shared<EnemyReport>(id_)),
	behaviorTree(std::make_unique<SelectorNode>()),
	attackCollider(attackCollider_),
	justAvoidIgnitionCollider(justAvoidIgnitionCollider_)
{
	if (report_handler_ == nullptr)
	{
		DebugMessenger::LogError("EnemyReportHandlerにnullptrが渡されました");
	}
	else
	{
		report_handler_->AddReport(report);
	}

	// HACK: behaviorTreeの構成の仕方の変更
	
	RegisterMoveDicision();
	RegisterAttackDicision();
	RegisterIdleDicision();
	
	
}

EnemyBrain::~EnemyBrain() = default;

void EnemyBrain::SetReference(std::shared_ptr<ObjectBase> owner_, std::shared_ptr<Animator> animator_, std::shared_ptr<Transform> transform_)
{
	owner = owner_;
	animator = animator_;
	transform = transform_;
	report->SetTransform(transform_);
}

void EnemyBrain::SetLocalTimeScale(float time_scale_)
{
	if (enableColliderTimer != nullptr)
	{
		enableColliderTimer->SetLocalTimeScale(time_scale_);
	}

	if (disableColliderTimer != nullptr)
	{
		disableColliderTimer->SetLocalTimeScale(time_scale_);
	}
}

void EnemyBrain::Execute(float elapsed_time_)
{
	behaviorTree->Tick(elapsed_time_);

	if (isAttack == false)
	{
		animator->SetIsAllowedToTransitionSameCurrent(false);
		if (moveDir.x > sideAnimThreshold)
		{
			animator->SetNextAnim(AKind::WalkR);
		}
		else if (moveDir.x < -sideAnimThreshold)
		{
			animator->SetNextAnim(AKind::WalkL);
		}
		else if (moveDir.z < 0)
		{
			animator->SetNextAnim(AKind::WalkF);
		}
		else if (moveDir.z > 0)
		{
			animator->SetNextAnim(AKind::WalkB);
		}
		else
		{
			animator->SetNextAnim(AKind::Idle);
		}
		animator->SetIsAllowedToTransitionSameCurrent(true);

		// TODO: 別関数にまとめる
		if (moveDir != Vector3::ZERO)
		{
			transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);
		}
		transform->Translate(moveDir);

	}
}
Status EnemyBrain::Turn(float elapsed_time_)
{
	Vector3 vec_norm_enemy_to_player = (directive->GetPlayerPosition() - transform->Position).Normalize();
	float dot = Vector3::Dot(transform->GetForward(), vec_norm_enemy_to_player);
	float angle = acosf(dot) * 180.0f / DX_PI;
	// 十分プレイヤーの方を向いていれば成功
	if (angle < turnThreshold)
	{
		return Status::Success;
	}
	animator->SetIsAllowedToTransitionSameCurrent(false);
	animator->SetNextAnim(AKind::WalkF);
	animator->SetIsAllowedToTransitionSameCurrent(true);
	transform->Translate(Vector3::FORWARD * moveForwardSpeed);
	transform->StartSlearpByForwardAndAngularVelocity(vec_norm_enemy_to_player, rotateSpeed);
	return Status::Running;
}


Status EnemyBrain::AttackStart(AKind anim_kind_)
{
	transform->Translate(Vector3::ZERO);
	animator->SetNextAnim(anim_kind_);
	enableColliderTimer = TimerFactory::CreateTimer(animator->GetActivationTime(), owner.lock(), this, &EnemyAI::EnemyBrain::EnableAttackCollider);
	disableColliderTimer = TimerFactory::CreateTimer(animator->GetDeactivationTime(), owner.lock(), this, &EnemyAI::EnemyBrain::DisableAttackCollider);

	return Status::Success;
}


void EnemyBrain::RegisterMoveDicision()
{
	std::unique_ptr<LeafNode> move_start = 
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{
				if (IsNullPtrToComponent()) { return Status::Failure; }
				if ((directive->GetDirectedPosition(id) - transform->Position).sqrLength() < moveStartThreshold * moveStartThreshold)
				{
					return Status::Failure;
				}

				return Status::Success;
			});

	std::unique_ptr<LeafNode> move_out_range = 
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{ 
				float maximum_distance = directive->GetMaximumDistance();
				if ((directive->GetPlayerPosition() - transform->Position).sqrLength() < maximum_distance * maximum_distance)
				{
					return Status::Failure;
				}
				moveDir = Vector3::FORWARD * moveForwardSpeed;
				return Status::Success;
			});

	std::unique_ptr<LeafNode> move_side =
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{
				// 現在のプレイヤー位置から見た相対位置
				Vector3 relative_position_to_player = transform->Position - directive->GetPlayerPosition();

				Vector3 directed_relative_position_to_player = directive->GetDirectedRelativePositionToPlayer(id);
				// 現在の位置ベクトルから目標位置ベクトルへの垂線ベクトル
				Vector3 perpendicular = relative_position_to_player.Projection(directed_relative_position_to_player) - relative_position_to_player;

				if (perpendicular.sqrLength() < moveEndThreshold * moveEndThreshold &&
					Vector3::Dot(relative_position_to_player, directed_relative_position_to_player) > 0)
				{
					moveDir.x = 0;
					return Status::Success;
				}

				if (relative_position_to_player.JudgeLeftOrRight(directed_relative_position_to_player) > 0)
				{
					moveDir -= Vector3::RIGHT * moveAcceleration * elapsed_time_;
					if (-moveDir.x > moveSideSpeed)
					{
						moveDir.x = -moveSideSpeed;
					}
				}
				else
				{
					moveDir += Vector3::RIGHT * moveAcceleration * elapsed_time_;
					if (moveDir.x > moveSideSpeed)
					{
						moveDir.x = moveSideSpeed;
					}
				}
				return Status::Success;
			});

	std::unique_ptr<LeafNode> move_forward_and_backward =
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{
				float length_between_directed_and_player = directive->GetDirectedRelativePositionToPlayer(id).Length();
				float length_between_this_and_player = (transform->Position - directive->GetPlayerPosition()).Length();
				if (fabsf(length_between_directed_and_player - length_between_this_and_player) < moveEndThreshold)
				{
					return Status::Success;
				}

				if (length_between_directed_and_player < length_between_this_and_player)
				{
					moveDir += Vector3::FORWARD * moveAcceleration * elapsed_time_;
					if (-moveDir.z > moveForwardSpeed)
					{
						moveDir.z = -moveForwardSpeed;
					}
				}
				else
				{
					moveDir -= Vector3::FORWARD * moveAcceleration * elapsed_time_;
					if (moveDir.z > moveBackwardSpeed)
					{
						moveDir.z = moveForwardSpeed;
					}
				}

				return Status::Success;
			});

	std::unique_ptr<CompositeNode> move_in_range = std::make_unique<SequenceNode>();
	move_in_range->AddNode(std::move(move_side));
	move_in_range->AddNode(std::move(move_forward_and_backward));

	// 移動方向セレクターを作成・ノード登録
	std::unique_ptr<CompositeNode> move_selector = std::make_unique<SelectorNode>();
	move_selector->AddNode(std::move(move_out_range));
	move_selector->AddNode(std::move(move_in_range));

	// 移動シークエンスの作成・登録
	std::unique_ptr<CompositeNode> move_sequence = std::make_unique<SequenceNode>();
	move_sequence->AddNode(std::move(move_start));
	move_sequence->AddNode(std::move(move_selector));

	behaviorTree->AddNode(std::move(move_sequence));
}


void EnemyBrain::RegisterAttackDicision()
{
	// 攻撃の末端ノードを作成
	std::unique_ptr<LeafNode> check_attackable = 
		std::make_unique<LeafNode>([this](float elapsed_time_) 
			{
				if (IsNullPtrToComponent()) { return Status::Failure; }
				if (directive->GetDirectedAciton(id) == Actions::STAND_BY || report->HasAchieved())
				{
					return Status::Failure;
				}
				isAttack = true;
				moveDir = Vector3::ZERO;
				DebugMessenger::Log("エネミー：攻撃シークエンス開始");
				return Status::Success;
			});

	std::unique_ptr<LeafNode> turn = 
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{ 
				return Turn(elapsed_time_); 
			});

	std::unique_ptr<LeafNode> attack_start0 = 
		std::make_unique<LeafNode>([this](float elapsed_time_) 
			{ 
				if (directive->GetDirectedAciton(id) != Actions::ATTACK0)
				{
					return Status::Failure;
				}
				return AttackStart(AKind::Attack00);
			});

	std::unique_ptr<LeafNode> attack_start1 = 
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{ 
				if (directive->GetDirectedAciton(id) != Actions::ATTACK1)
				{
					return Status::Failure;
				}
				return AttackStart(AKind::Attack01);
			});

	std::unique_ptr<LeafNode> attack_start2 = 
		std::make_unique<LeafNode>([this](float elapsed_time_) 
			{
				if (directive->GetDirectedAciton(id) != Actions::ATTACK2)
				{
					return Status::Failure;
				}
				return AttackStart(AKind::Attack02);
			});

	std::unique_ptr<LeafNode> attack =
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{ 
				if (animator->GetTimeUntilStartTransition() < 0)
				{
					DebugMessenger::Log("エネミー：攻撃シークエンス終了");
					if (directive->GetDirectedAciton(id) != Actions::STAND_BY)
					{
						report->AchieveMission();
					}
					isAttack = false;
					return Status::Success;
				}

				return Status::Running;
			});

	// 攻撃セレクターの作成・ノード登録
	std::unique_ptr<CompositeNode> attack_selector = std::make_unique<SelectorNode>();
	attack_selector->AddNode(std::move(attack_start0));
	attack_selector->AddNode(std::move(attack_start1));
	attack_selector->AddNode(std::move(attack_start2));


	// 攻撃シークエンスの作成・ノード登録
	std::unique_ptr<CompositeNode> attack_sequence = std::make_unique<SequenceNode>();
	attack_sequence->AddNode(std::move(check_attackable));
	attack_sequence->AddNode(std::move(turn));
	attack_sequence->AddNode(std::move(attack_selector));
	attack_sequence->AddNode(std::move(attack));


	behaviorTree->AddNode(std::move(attack_sequence));

}

void EnemyBrain::RegisterIdleDicision()
{
	// 待機の末端ノードを作成
	std::unique_ptr<LeafNode> idle = 
		std::make_unique<LeafNode>([this](float elapsed_time_)
			{
				if (directive->GetDirectedAciton(id) == Actions::STAND_BY)
				{
					report->AchieveMission();
				}

				if ((directive->GetDirectedPosition(id) - transform->Position).sqrLength() < moveEndThreshold * moveEndThreshold)
				{
					moveDir = Vector3::ZERO;
				}

				return Status::Success;
			});

	// rootに登録
	behaviorTree->AddNode(std::move(idle));
}

void EnemyBrain::EnableAttackCollider()
{
	attackCollider->SetIsEnabled(true);
	justAvoidIgnitionCollider->SetIsEnabled(true);
}

void EnemyBrain::DisableAttackCollider()
{
	attackCollider->SetIsEnabled(false);
	justAvoidIgnitionCollider->SetIsEnabled(false);
}

bool EnemyBrain::IsNullPtrToComponent()
{
	if (transform == nullptr)
	{
		DebugMessenger::LogError("transform が nullptrです");
		return true;
	}
	if (directive == nullptr)
	{ 
		DebugMessenger::LogError("direcive が nullptrです");
		return true;
	}

	return false;
}

#ifdef DEBUG
void EnemyBrain::DebugDrow()
{
	unsigned int color = GetColor(150, 255, 150);
	DrawSphere3D(directive->GetDirectedPosition(id).ToVECTOR(), 1.0f, 8, color, color, TRUE);
}
#endif