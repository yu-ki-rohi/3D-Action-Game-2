#pragma once

// Modelの種類
enum class MKind
{
	Player,
	Enemy,
	Stage
};

// Animationの種類
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

// Vertex Shaderの種類
enum class VSKind
{
	Rigidbody,
	RigidbodyShadow,
	SkinnedMesh,
	SkinnedMeshShadow
};

// Pixel Shaderの種類
enum class PSKind
{
	Phong,
	ShadowMap,
	PostEffect
};