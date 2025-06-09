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
		Stage
	};

public:
	bool IsActive() { return isActive; }

public:
	// アクセサ
	virtual Tag GetTag() = 0;
	// ChatGPTを使用して生成したコード
	template<typename T>
	std::shared_ptr<T> GetComponent()
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


	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;



protected:
	bool isActive = true;
	std::vector<std::shared_ptr<ComponentBase>> components;
};