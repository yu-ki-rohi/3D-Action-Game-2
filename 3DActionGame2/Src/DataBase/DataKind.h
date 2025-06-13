#pragma once

// Model‚ÌŽí—Þ
enum class MKind
{
	Player,
	Enemy,
	Stage
};

// Animation‚ÌŽí—Þ
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

// Vertex Shader‚ÌŽí—Þ
enum class VSKind
{
	Rigidbody,
	RigidbodyShadow,
	SkinnedMesh,
	SkinnedMeshShadow
};

// Pixel Shader‚ÌŽí—Þ
enum class PSKind
{
	Phong,
	ShadowMap,
	PostEffect
};