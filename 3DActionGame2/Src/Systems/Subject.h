#pragma once
#include <vector>
#include <memory>

class ObserverBase;
class Subject
{
public:
	// オブザーバーの追加
	void AddObserver(std::weak_ptr<ObserverBase> observer_);
	// オブザーバーを削除
	void RemoveObserver(std::shared_ptr<ObserverBase> observer_);

	// オブザーバーに通知
	void NotifyObservers();
	void NotifyObservers(unsigned char uc_);
	void NotifyObservers(float f1_, float f2_);

	// オブザーバー全削除
	void Clear();
private:
	std::vector<std::weak_ptr<ObserverBase>> observers;
};