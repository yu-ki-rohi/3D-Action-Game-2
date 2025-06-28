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
		Player = 1 << 0,
		Enemy = 1 << 1,
		Stage = 1 << 2,
		Camera = 1 << 3,
		Effect = 1 << 4
	};
public:
	ObjectBase() : isActive(true) {}
	~ObjectBase() = default;

public:
	// �A�N�Z�T
	virtual Tag GetTag() const = 0;
	bool IsActive() const { return isActive; }

	// ChatGPT���g�p���Đ��������R�[�h
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

	// ���������shared_from_this()���g���ēn�����߂ɗp��
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
	// ���҂ɓn�����Ƃ�����R���|�[�l���g�������Ɋi�[
	std::vector<std::shared_ptr<ComponentBase>> components;
};