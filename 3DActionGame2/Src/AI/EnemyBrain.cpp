#include "EnemyBrain.h"
#include "EnemyCommander.h"
#include "IEnemyDirectiveReader.h"
#include "../Objects/Components/Transform.h"
#include "../Objects/Components/Animator.h"
#include "../Systems/TimerFactory.h"
#include "../Collision/Collider.h"
#include "BehaviorTree/SelectorNode.h"
#include "BehaviorTree/SequenceNode.h"
#include "BehaviorTree/RandomNode.h"
#include "BehaviorTree/LeafNode.h"
#include "../Debug/DebugMessenger.h"

using namespace BehaviorTree;
using namespace EnemyAI;
EnemyBrain::EnemyBrain(
	int id_,
	std::shared_ptr<IEnemyDirectiveReader> directive_,
	std::shared_ptr<Collider> attackCollider_,
	std::shared_ptr<Collider> justAvoidIgnitionCollider_) :
	id(id_),
	directive(directive_),
	behaviorTree(std::make_unique<SelectorNode>()),
	attackCollider(attackCollider_),
	justAvoidIgnitionCollider(justAvoidIgnitionCollider_)
{
	// HACK: behaviorTreeの構成の仕方の変更
	
	// 移動ノードの末端ノードを作成
	std::unique_ptr<LeafNode> moveStart = std::make_unique<LeafNode>([this](float elapsed_time_) { return MoveStart(elapsed_time_); });
	std::unique_ptr<LeafNode> move = std::make_unique<LeafNode>([this](float elapsed_time_) { return Move(elapsed_time_); });

	// 移動シークエンスの作成・登録
	std::unique_ptr<CompositeNode> moveSequence = std::make_unique<SequenceNode>();
	moveSequence->AddNode(std::move(moveStart));
	moveSequence->AddNode(std::move(move));


	// 攻撃0ノードの末端ノードを作成
	std::unique_ptr<LeafNode> attackStart0 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart0(elapsed_time_); });
	std::unique_ptr<LeafNode> attack0 = std::make_unique<LeafNode>([this](float elapsed_time_) { return Attack(elapsed_time_); });

	// 攻撃0シークエンスの作成・登録
	std::unique_ptr<CompositeNode> attackSequence0 = std::make_unique<SequenceNode>();
	attackSequence0->AddNode(std::move(attackStart0));
	attackSequence0->AddNode(std::move(attack0));

	
	// 攻撃1ノードの末端ノードを作成
	std::unique_ptr<LeafNode> attackStart1 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart1(elapsed_time_); });
	std::unique_ptr<LeafNode> attack1 = std::make_unique<LeafNode>([this](float elapsed_time_) { return Attack(elapsed_time_); });

	// 攻撃1シークエンスの作成・登録
	std::unique_ptr<CompositeNode> attackSequence1 = std::make_unique<SequenceNode>();
	attackSequence1->AddNode(std::move(attackStart1));
	attackSequence1->AddNode(std::move(attack1));

	
	// 攻撃2ノードの末端ノードを作成
	std::unique_ptr<LeafNode> attackStart2 = std::make_unique<LeafNode>([this](float elapsed_time_) { return AttackStart2(elapsed_time_); });
	std::unique_ptr<LeafNode> attack2 = std::make_unique<LeafNode>([this](float elapsed_time_) { return Attack(elapsed_time_); });

	// 攻撃2シークエンスの作成・登録
	std::unique_ptr<CompositeNode> attackSequence2 = std::make_unique<SequenceNode>();
	attackSequence2->AddNode(std::move(attackStart2));
	attackSequence2->AddNode(std::move(attack2));

	// ランダム攻撃ノードの作成・登録
	std::unique_ptr<CompositeNode> attackRandom = std::make_unique<RandomNode>();

	attackRandom->AddNode(std::move(attackSequence0));
	attackRandom->AddNode(std::move(attackSequence1));
	attackRandom->AddNode(std::move(attackSequence2));

	std::unique_ptr<LeafNode> idle = std::make_unique<LeafNode>([this](float elapsed_time_) { return Idle(elapsed_time_); });

	// rootに登録
	behaviorTree->AddNode(std::move(moveSequence));
	behaviorTree->AddNode(std::move(attackRandom));
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

	animator->SetNextAnim(AKind::WalkF);

	transform->SetForward(directive->GetDirectedPosition(id) - transform->Position);
	transform->StartSlearpByForwardAndAngularVelocity(directive->GetDirectedPosition(id) - transform->Position, 150.0f);

	DebugMessenger::Log("エネミー：移動シークエンス開始");
	return Status::Success;
}

Status EnemyBrain::Move(float elapsed_time_)
{
	if ((directive->GetDirectedPosition(id) - transform->Position).sqrLength() < moveEndThreshold * moveEndThreshold)
	{
		DebugMessenger::Log("エネミー：移動シークエンス終了");
		return Status::Success;
	}

	transform->SetForward(directive->GetDirectedPosition(id) - transform->Position);
	transform->StartSlearpByForwardAndAngularVelocity(directive->GetDirectedPosition(id) - transform->Position, 150.0f);

	transform->Position += transform->GetForward() * moveSpeed * elapsed_time_;
	return Status::Running;
}

Status EnemyBrain::Idle(float elapsed_time_)
{
	animator->SetIsAllowedToTransitionSameCurrent(false);
	animator->SetNextAnim(AKind::Idle);
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
	if (IsNullPtrToComponent()) { return Status::Failure; }

	if (directive->GetDirectedAciton(id) != Actions::ATTACK)
	{
		return Status::Failure;
	}

	DebugMessenger::Log("エネミー：攻撃シークエンス開始");
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