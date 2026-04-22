#pragma once
#include <memory>

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	enum class Step
	{
		Init,
		Load,
		Update,
		Finish
	};

	enum class Type
	{
		Title,
		Game,
		Finish
	};
public:
	Step GetCurrentStep()
	{
		return currentStep;
	}

public:
	virtual ~SceneBase() = default;

public:
	// Step::Updateで行われる処理
	virtual void Start() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time_) = 0;
	virtual void Render() = 0;

	// Step::Loadで行われる処理
	virtual void UpdateInLoading(float elapsed_time_) = 0;
	virtual void RenderInLoading() = 0;

	// Sceneの初期化処理
	virtual void Initialize() = 0;

	/// <summary>
	/// 現在Sceneの削除処理
	/// </summary>
	/// <returns>次のシーンType</returns>
	virtual Type Delete() = 0;

	// インゲームでのジャスト回避時の演出
	// スマートポインターで呼び出すためにこちらで宣言
	// ただこのためだけにここに書くのは微妙な気がする
	// MFPC使うなどすれば別にこちらで宣言しなくてもいいかも?
	virtual void SuccessJustAvoid(){}

protected:
	Step currentStep = Step::Init;

};