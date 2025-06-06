#pragma once
#include <vector>
#include <memory>

class Collider;
class ComponentBase;

class ObjectBase
{
public:
	bool IsActive() { return isActive; }

public:
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;

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
	}

protected:
	bool isActive = true;
	std::vector<std::shared_ptr<ComponentBase>> components;
};