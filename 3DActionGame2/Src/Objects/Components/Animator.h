#pragma once
#include <unordered_map>
#include "ComponentBase.h"

class Animator : public ComponentBase
{
public:
	enum class AnimType
	{
		Idle,
		WalkF,
		WalkL,
		WalkR,
		WalkB,
		Run,
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

public:
	Animator(int idle_anim_handle_);

public:
	// animHandlesにキャッシュとして登録
	void RegisterAnim(AnimType anim_type_, int anim_handle_);

	void SetNextAnim(AnimType anim_type_, bool is_loop_ = false);
	/// <summary>
	/// アニメーション遷移先をセット
	/// </summary>
	/// <param name="anim_type_">遷移先の種類</param>
	/// <param name="start_changing_time_">遷移を開始するアニメーション時刻</param>
	/// <param name="changing_time_">遷移し終わるまでの時間</param>
	/// <param name="is_loop_">次のアニメーションをループさせるか否か</param>
	void SetNextAnim(AnimType anim_type_, float start_changing_time_, float changing_time_, bool is_loop_ = false);

	void SetAnimSpeed(float anim_speed_);

public:

	void SetupRenderAnim(int model_handle_);
	void DetachAnim(int model_handle_);

	void Update(float elapsed_time_);

private:
	// AnimTypeごとのハンドル
	std::unordered_map<AnimType, int> animHandles;

	// nextXXはアニメーション遷移先を予約するために使用
	// (アニメーションの線形補間のため)

	// 再生するアニメーションのハンドル
	// AnimTypeを控えておく方法でもいいかも
	int currentAnimHandle = -1;
	int nextAnimHandle = -1;

	int currentAttachIndex = -1;
	int nextAttachIndex = -1;

	float currentAnimTimer = 0.0f;
	float nextAnimTimer = 0.0f;

	// 現在アニメーションのトータル時間
	float animTime = 0.0f;

	// 一旦固定値
	// TODO アニメーションごとに設定できるようにする
	float animSpeed = 30.0f;

	float startChangingTime = 0.0f;
	float changingTime = 0.1f;

	bool isLoop = true;
	bool nextIsLoop = false;

	// 切替の始動/終了を通知するため
	bool isStartChangeAnimNow = false;
	bool isFinishChangeAnimNow = false;

private:
	static constexpr float minChangingTime = 0.001f;

};