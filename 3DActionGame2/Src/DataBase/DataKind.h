#pragma once

// Modelの種類
enum class MKind
{
	Player,
	Enemy,
	Stage
};

// Animationの種類
enum class AKind
{
	Idle,
	WalkF,
	WalkL,
	WalkR,
	WalkB,
	Run,
	Avoid,
	Attack00,
	Attack01,
	Attack02,
	Attack03,
	Attack04,
	Attack05,
	Attack06,
	Damage,
	Down
};

// アニメーションの各種パラメータ
// 1. アニメーション全体の時間, 
// 2. 前アニメーションとの合成にかける時間, 
// 3. 次アニメーションとの合成を始める時間,
// 4. 次アニメーションとの合成にかける時間,
// 5. 攻撃・無敵などを有効化する時刻,
// 6. 攻撃・無敵などを有効化し続ける時間,
// 7. 再生開始位置の調整, 
// 8. 再生終了位置の調整,
// 9. ループさせるかどうか
struct AnimationParameters
{
	// アニメーション全体の時間
	const float Duration;

	// 前アニメーションとの合成にかける時間
	const float TransitionInDuration;

	// 次アニメーションとの合成を始める時間
	const float TransitionOutStartTime;

	// 次アニメーションとの合成にかける時間
	const float TransitionOutDuration;

	// 攻撃・無敵などを有効化する時刻
	const float ActivationTime;

	// 攻撃・無敵などを有効化し続ける時間
	const float ActivationDuration;

	// 再生開始位置
	const float StartOffset;

	// 再生終了位置の調整
	const float EndOffset;

	const bool IsLoop;

	AnimationParameters(
		float animation_duration_,
		float transition_in_duration_,
		float transition_out_startTime_,
		float transition_out_duration_,
		float activation_time_,
		float activation_duration_,
		float animation_start_offset_,
		float animation_end_offset,
		bool is_loop_
	) :
		Duration(animation_duration_),
		TransitionInDuration(transition_in_duration_),
		TransitionOutStartTime(transition_out_startTime_),
		TransitionOutDuration(transition_out_duration_),
		ActivationTime(activation_time_),
		ActivationDuration(activation_duration_),
		StartOffset(animation_start_offset_),
		EndOffset(animation_end_offset),
		IsLoop(is_loop_)
	{

	}

};


// Vertex Shaderの種類
enum class VSKind
{
	Rigidbody,
	RigidbodyShadow,
	SkinnedMesh,
	SkinnedMeshShadow
};

// Pixel Shaderの種類
enum class PSKind
{
	NoLighting,
	Phong,
	ShadowMap,
	PostEffect
};

// Effect の種類
enum class EKind
{
	HitSlash
};

enum class BGMKind
{
	Main
};

enum class SEKind
{
	SystemDecide,
	SystemCancel,
	SystemSelect,
	HitSlash
};