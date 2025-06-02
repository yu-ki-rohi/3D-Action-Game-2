#pragma once
#include "InputBehavior.h"
#include "../Systems/Subject.h"

/// <summary>
/// 数値の通知が必要な場合に使用(StickやTrigger)
/// </summary>
class InputBehaviorWithSubject : public InputBehavior
{
public:
	void AddObserver(std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(std::shared_ptr<ObserverBase> observer_);

	void ClearObservers();
protected:
	Subject subject;
};