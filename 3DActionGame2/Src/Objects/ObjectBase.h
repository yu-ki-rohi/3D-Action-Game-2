#pragma once

class Collider;

class ObjectBase
{
public:
	bool IsActive();

public:
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;

	// “–‚½‚è”»’èEŠî’ê‚Å‚Í‹óŠÖ”
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual void OnCollisionEnter(Collider* collider);
	virtual void OnCollisionStay(Collider* collider);
	virtual void OnCollisionExit(Collider* collider);

protected:
	bool isActive = true;
};