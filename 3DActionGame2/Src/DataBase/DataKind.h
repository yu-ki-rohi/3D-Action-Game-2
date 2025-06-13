#pragma once

// Model�̎��
enum class MKind
{
	Player,
	Enemy,
	Stage
};

// Animation�̎��
enum class AKind
{
	Idle,
	WalkF,
	WalkL,
	WalkR,
	WalkB,
	Run,
	Attack00,
	Attack01,
	Attack02,
	Attack03,
	Attack04,
	Attack05,
	Attack06,
	Damage,
	Down
};

// Vertex Shader�̎��
enum class VSKind
{
	Rigidbody,
	RigidbodyShadow,
	SkinnedMesh,
	SkinnedMeshShadow
};

// Pixel Shader�̎��
enum class PSKind
{
	Phong,
	ShadowMap,
	PostEffect
};