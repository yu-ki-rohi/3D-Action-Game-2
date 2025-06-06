#pragma once
#include <vector>
#include <memory>

class ObserverBase;
class Subject
{
public:
	// �I�u�U�[�o�[�̒ǉ�
	void AddObserver(std::weak_ptr<ObserverBase> observer_);
	// �I�u�U�[�o�[���폜
	void RemoveObserver(std::shared_ptr<ObserverBase> observer_);

	// �I�u�U�[�o�[�ɒʒm
	void NotifyObservers();
	void NotifyObservers(unsigned char uc_);
	void NotifyObservers(float f1_, float f2_);

	// �I�u�U�[�o�[�S�폜
	void Clear();
private:
	std::vector<std::weak_ptr<ObserverBase>> observers;
};