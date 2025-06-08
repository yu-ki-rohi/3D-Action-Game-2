#pragma once

class Collider;

class ComponentBase
{
public:
	// “–‚½‚è”»’èEŠî’ê‚Å‚Í‹óŠÖ”
	virtual void OnTriggerEnter(const Collider* other_) {}
	virtual void OnTriggerStay(const Collider* other_) {}
	virtual void OnTriggerExit(const Collider* other_) {}

	virtual void OnCollisionEnter(const Collider* other_) {}
	virtual void OnCollisionStay(const Collider* other_) {}
	virtual void OnCollisionExit(const Collider* other_) {}

};