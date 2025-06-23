#pragma once
#include "InputBehavior.h"
#include "../Systems/Subject.h"

/// <summary>
/// ���l�̒ʒm���K�v�ȏꍇ�Ɏg�p(Stick��Trigger)
/// </summary>
class InputBehaviorWithSubject : public InputBehavior
{
public:
	void AddObserver(InputManager::Map map_, std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(InputManager::Map map_, std::shared_ptr<ObserverBase> observer_);

	void ClearObservers();
protected:
	std::unordered_map<InputManager::Map, Subject> subjects;
};