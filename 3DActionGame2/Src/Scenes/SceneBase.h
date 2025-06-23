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
		Sample,
		Finish
	};
public:
	Step GetCurrentStep()
	{
		return currentStep;
	}

public:
	// 各frameの最初に行われる処理
	virtual void Start() = 0;

	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time_) = 0;
	virtual void Render() = 0;

	virtual void UpdateInLoading(float elapsed_time_) = 0;
	virtual void RenderInLoading() = 0;

	// Sceneの初期化処理
	virtual void Initialize() = 0;

	/// <summary>
	/// 現在Sceneの削除処理
	/// </summary>
	/// <returns>次のシーンType</returns>
	virtual Type Delete() = 0;

	virtual void SuccessJustAvoid(){}

protected:
	Step currentStep = Step::Init;

};