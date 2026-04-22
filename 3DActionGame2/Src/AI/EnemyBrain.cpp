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
	
	// 移動の末端ノードを作成
	std::unique_ptr<LeafNode> moveStart = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveStart(elapsed_time_); });
	std::unique_ptr<LeafNode> moveBackwardStart = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveBackwardStart(elapsed_time_); });
	std::unique_ptr<LeafNode> moveBackward = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveBackward(elapsed_time_); });
	std::unique_ptr<LeafNode> moveSideStart = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveSideStart(elapsed_time_); });
	std::unique_ptr<LeafNode> moveSide = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveSide(elapsed_time_); });
	std::unique_ptr<LeafNode> moveForwardStart = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveForwardStart(elapsed_time_); });
	std::unique_ptr<LeafNode> moveForward = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveForward(elapsed_time_); });

	// 後方移動シークエンス作成・ノード登録
	std::unique_ptr<CompositeNode> moveBackwardSequence = std::make_unique<SequenceNode>();
	moveBackwardSequence->AddNode(std::move(moveBackwardStart));
	moveBackwardSequence->AddNode(std::move(moveBackward));

	// 横移動シークエンス作成・ノード登録
	std::unique_ptr<CompositeNode> moveSideSequence = std::make_unique<SequenceNode>();
	moveSideSequence->AddNode(std::move(moveSideStart));
	moveSideSequence->AddNode(std::move(moveSide));

	// 前方移動シークエンス作成・ノード登録
	std::unique_ptr<CompositeNode> moveForwardSequence = std::make_unique<SequenceNode>();
	moveForwardSequence->AddNode(std::move(moveForwardStart));
	moveForwardSequence->AddNode(std::move(moveForward));

	// 移動方向セレクターを作成・ノード登録
	std::unique_ptr<SelectorNode> moveDirSelector = std::make_unique<SelectorNode>();
	moveDirSelector->AddNode(std::move(moveBackwardSequence));
	moveDirSelector->AddNode(std::move(moveSideSequence));
	moveDirSelector->AddNode(std::move(moveForwardSequence));

	// 移動シークエンスの作成・登録
	std::unique_ptr<CompositeNode> moveSequence = std::make_unique<SequenceNode>();
	moveSequence->AddNode(std::move(moveStart));
	moveSequence->AddNode(std::move(moveDirSelector));


	// 攻撃の末端ノードを作成
	std::unique_ptr<LeafNode> checkAttackable = std::make_unique<LeafNode>([this](float elapsed_time_) { return CheckAttackable(elapsed_time_); });
	std::unique_ptr<LeafNode> turnToAttack = std::make_unique<LeafNode>([this](float elapsed_time_) { return Turn(elapsed_time_); });
	std::unique_ptr<LeafNode> attackStart0 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart0(elapsed_time_); });
	std::unique_ptr<LeafNode> attackStart1 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart1(elapsed_time_); });
	std::unique_ptr<LeafNode> attackStart2 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart2(elapsed_time_); });
	std::unique_ptr<LeafNode> attack = std::make_unique<LeafNode>([this](float elapsed_time_) { return Attack(elapsed_time_); });
	
	// ランダム攻撃ノードの作成・ノード登録
	std::unique_ptr<CompositeNode> attackRandom = std::make_unique<RandomNode>();
	attackRandom->AddNode(std::move(attackStart0));
	attackRandom->AddNode(std::move(attackStart1));
	attackRandom->AddNode(std::move(attackStart2));


	// 攻撃シークエンスの作成・ノード登録
	std::unique_ptr<CompositeNode> attackSequence = std::make_unique<SequenceNode>();
	attackSequence->AddNode(std::move(checkAttackable));
	attackSequence->AddNode(std::move(turnToAttack));
	attackSequence->AddNode(std::move(attackRandom));
	attackSequence->AddNode(std::move(attack));

	
	// 待機の末端ノードを作成
	std::unique_ptr<LeafNode> idle = std::make_unique<LeafNode>([this](float elapsed_time_) { return Idle(elapsed_time_); });

	// rootに登録
	behaviorTree->AddNode(std::move(moveSequence));
	behaviorTree->AddNode(std::move(attackSequence));
	behaviorTree->AddNode(std::move(idle));
}

EnemyBrain::~EnemyBrain() = default;

void EnemyBrain::SetReference(std::shared_ptr<ObjectBase> owner_, std::shared_ptr<Animator> animator_, std::shared_ptr<Transform> transform_)
{
	owner = owner_;
	animator = animator_;
	transform = transform_;
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
}

Status EnemyBrain::MoveStart(float elapsed_time_)
{
	if (IsNullPtrToComponent()) { return Status::Failure; }

	if ((directive->GetDirectedPosition(id) - transform->Position).sqrLength() < moveStartThreshold * moveStartThreshold)
	{
		return Status::Failure;
	}
	DebugMessenger::Log("エネミー：移動シークエンス開始");
	return Status::Success;
}

Status EnemyBrain::MoveForwardStart(float elapsed_time_)
{
	animator->SetNextAnim(AKind::WalkF);

	transform->StartSlearpByForwardAndAngularVelocity(directive->GetDirectedPosition(id) - transform->Position, rotateSpeed);

	return Status::Success;
}

Status EnemyBrain::MoveForward(float elapsed_time_)
{
	if ((directive->GetDirectedPosition(id) - transform->Position).sqrLength() < moveEndThreshold * moveEndThreshold)
	{
		return Status::Success;
	}

	transform->StartSlearpByForwardAndAngularVelocity(directive->GetDirectedPosition(id) - transform->Position, rotateSpeed);

	transform->Position += transform->GetForward() * moveForwardSpeed * elapsed_time_;
	return Status::Running;
}

Status EnemyBrain::MoveSideStart(float elapsed_time_)
{
	// 現在のプレイヤー位置から見た相対位置
	Vector3 relative_position_to_player = transform->Position - directive->GetPlayerPosition();
	// 現在の位置ベクトルから目標位置ベクトルへの垂線ベクトル
	Vector3 perpendicular = relative_position_to_player.Projection(directive->GetDirectedRelativePositionToPlayer(id)) - relative_position_to_player;
	
	// 垂線が十分短い且つ相対位置が指示座標と同方向の時
	if (perpendicular.sqrLength() < moveEndThreshold * moveEndThreshold &&
		Vector3::Dot(relative_position_to_player, directive->GetDirectedRelativePositionToPlayer(id)) > 0)
	{
		return Status::Failure;
	}

	transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);

	if (relative_position_to_player.JudgeLeftOrRight(directive->GetDirectedRelativePositionToPlayer(id)) > 0)
	{
		animator->SetNextAnim(AKind::WalkL);
	}
	else
	{
		animator->SetNextAnim(AKind::WalkR);
	}
	return Status::Success;
}

Status EnemyBrain::MoveSide(float elapsed_time_)
{
	// 現在のプレイヤー位置から見た相対位置
	Vector3 relative_position_to_player = transform->Position - directive->GetPlayerPosition();
	// 現在の位置ベクトルから目標位置ベクトルへの垂線ベクトル
	Vector3 perpendicular = relative_position_to_player.Projection(directive->GetDirectedRelativePositionToPlayer(id)) - relative_position_to_player;

	// 垂線が十分短い且つ相対位置が指示座標と同方向の時
	if (perpendicular.sqrLength() < moveEndThreshold * moveEndThreshold &&
		Vector3::Dot(relative_position_to_player, directive->GetDirectedRelativePositionToPlayer(id)) > 0)
	{
		return Status::Success;
	}

	transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);

	if (relative_position_to_player.JudgeLeftOrRight(directive->GetDirectedRelativePositionToPlayer(id)) > 0)
	{
		animator->SetIsAllowedToTransitionSameCurrent(false);
		animator->SetNextAnim(AKind::WalkL);
		animator->SetIsAllowedToTransitionSameCurrent(true);
		transform->Position -= transform->GetRight() * moveSideSpeed * elapsed_time_;
	}
	else
	{
		animator->SetIsAllowedToTransitionSameCurrent(false);
		animator->SetNextAnim(AKind::WalkR);
		animator->SetIsAllowedToTransitionSameCurrent(true);
		transform->Position += transform->GetRight() * moveSideSpeed * elapsed_time_;
	}
	return Status::Running;
}

Status EnemyBrain::MoveBackwardStart(float elapsed_time_)
{
	// 「自身とプレイヤーの距離」が「指示位置とプレイヤーの距離」より離れているときは即終了
	if (directive->GetDirectedRelativePositionToPlayer(id).sqrLength() < (transform->Position - directive->GetPlayerPosition()).sqrLength())
	{
		return Status::Failure;
	}
	transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);
	animator->SetNextAnim(AKind::WalkB);
	return Status::Success;
}

Status EnemyBrain::MoveBackward(float elapsed_time_)
{
	// 「自身とプレイヤーの距離」が「指示位置とプレイヤーの距離」より離れているときに終了
	if ((directive->GetDirectedPosition(id) - directive->GetPlayerPosition()).sqrLength() < (transform->Position - directive->GetPlayerPosition()).sqrLength())
	{
		return Status::Success;
	}

	transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);
	transform->Position -= transform->GetForward() * moveBackwardSpeed * elapsed_time_;
	return Status::Running;
}

Status EnemyBrain::Idle(float elapsed_time_)
{
	animator->SetIsAllowedToTransitionSameCurrent(false);
	animator->SetNextAnim(AKind::Idle);
	animator->SetIsAllowedToTransitionSameCurrent(true);
	transform->StartSlearpByForwardAndAngularVelocity(directive->GetPlayerPosition() - transform->Position, rotateSpeed);
	return Status::Success;
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
	transform->StartSlearpByForwardAndAngularVelocity(vec_norm_enemy_to_player, rotateSpeed);
	return Status::Running;
}

Status EnemyBrain::CheckAttackable(float elapsed_time_)
{
	if (IsNullPtrToComponent()) { return Status::Failure; }
	if (directive->GetDirectedAciton(id) != Actions::ATTACK)
	{
		return Status::Failure;
	}

	DebugMessenger::Log("エネミー：攻撃シークエンス開始");
	return Status::Success;
}
Status EnemyBrain::AttackStart0(float elapsed_time_)
{
	return AttackStart(AKind::Attack00);
}

Status EnemyBrain::AttackStart1(float elapsed_time_)
{
	return AttackStart(AKind::Attack01);
}

Status EnemyBrain::AttackStart2(float elapsed_time_)
{
	return AttackStart(AKind::Attack02);
}

Status EnemyBrain::AttackStart(AKind anim_kind_)
{
	animator->SetNextAnim(anim_kind_);
	enableColliderTimer = TimerFactory::CreateTimer(animator->GetActivationTime(), owner.lock(), this, &EnemyAI::EnemyBrain::EnableAttackCollider);
	disableColliderTimer = TimerFactory::CreateTimer(animator->GetDeactivationTime(), owner.lock(), this, &EnemyAI::EnemyBrain::DisableAttackCollider);

	return Status::Success;
}

Status EnemyBrain::Attack(float elapsed_time_)
{
	if (animator->GetTimeUntilStartTransition() < 0)
	{
		DebugMessenger::Log("エネミー：攻撃シークエンス終了");
		return Status::Success;
	}

	return Status::Running;
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