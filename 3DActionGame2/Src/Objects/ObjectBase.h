#pragma once
#include <vector>
#include <memory>

class Collider;
class ComponentBase;

class ObjectBase : public std::enable_shared_from_this<ObjectBase>
{
public:
	enum class Tag
	{
		Player,
		Enemy,
		Stage,
		Camera
	};
public:
	ObjectBase() : isActive(true) {}
	~ObjectBase() = default;

public:
	// アクセサ
	virtual Tag GetTag() const = 0;
	bool IsActive() const { return isActive; }

	// ChatGPTを使用して生成したコード
	template<typename T>
	std::shared_ptr<T> GetComponent() const
	{
		for (auto& comp : components)
		{
			if (auto casted = std::dynamic_pointer_cast<T>(comp))
			{
				return casted;
			}
		}
		return nullptr;
	}

	virtual void SetMonochrome(float rate_) = 0;
	virtual void SetLocalTimeScale(float time_scale_) = 0;

	// 生成直後にshared_from_this()を使って渡すために用意
	virtual void Start() = 0;

	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;

protected:
	void AddComponent(std::shared_ptr<ComponentBase> component_)
	{
		components.push_back(component_);
	}

protected:
	bool isActive;
	// 他者に渡すことがあるコンポーネントをここに格納
	std::vector<std::shared_ptr<ComponentBase>> components;
};