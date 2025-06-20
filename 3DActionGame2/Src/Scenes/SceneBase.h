#pragma once

class SceneBase
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
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time_) = 0;
	virtual void Render() = 0;

	virtual void UpdateInLoading(float elapsed_time_) = 0;
	virtual void RenderInLoading() = 0;

	virtual void Initialize() = 0;

	/// <summary>
	/// 現在Sceneの削除処理
	/// </summary>
	/// <returns>次のシーンType</returns>
	virtual Type Delete() = 0;

protected:
	Step currentStep = Step::Init;

};