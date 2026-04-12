#pragma once
#include <memory>

class Collider;

// 検討: コンポーネントに持たせる機能
// 現状当たり判定に関するものを持たせているが、これはインターフェースにした方が通りがいい気がする
// IUpdatableみたいなインターフェースの実装もありかな
class ComponentBase
{
public:
	// 当たり判定
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other_) {}
	virtual void OnTriggerStay(std::shared_ptr<Collider> other_){}
	virtual void OnTriggerExit(std::shared_ptr<Collider> other_){}

	virtual void OnCollisionEnter(std::shared_ptr<Collider> other_){}
	virtual void OnCollisionStay(std::shared_ptr<Collider> other_){}
	virtual void OnCollisionExit(std::shared_ptr<Collider> other_){}

};