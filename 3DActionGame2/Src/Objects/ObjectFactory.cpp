#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "../Assets/AssetsManager.h"
#include "../Scenes/GameScene.h"

#include "../Assets/AnimationResource.h"
#include "../Assets/ModelResource.h"
#include "../Assets/ShaderResource.h"

#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Animator.h"
#include "Components/PlayerStatus.h"
#include "Components/EnemyStatus.h"

//#include "TestObject.h"
#include "SampleCamera.h"
#include "SampleStage.h"
#include "SampleCharacter.h"

#include "CameraTPS.h"
#include "PlayerSaber.h"
#include "Enemy.h"

#include "FlashEffect.h"

ObjectFactory::ObjectFactory(std::shared_ptr<ObjectManager> object_manager_, std::shared_ptr<AssetsManager> assets_manager_, std::shared_ptr<ColliderRegisterInterface> collider_register_interface_) :
	objectManager(object_manager_),
	assetsManager(assets_manager_),
	colliderRegisterInterface(collider_register_interface_),
	isJustAvoidTime(false)

{

}

void ObjectFactory::SetIsJustAvoidTime(bool is_just_avoid_time_)
{
	isJustAvoidTime = is_just_avoid_time_;
}

// テスト用に作ったやつ
// 一応残してあるが、最終的には消す
std::shared_ptr<ObjectBase> ObjectFactory::Create()
{
#if false
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	if (object_manager == nullptr) return nullptr;
	std::shared_ptr<ObjectBase> obj = std::make_shared<TestObject>();
	object_manager->Register(obj);
	return obj;
#endif
	return nullptr;
}

std::shared_ptr<ObjectBase> ObjectFactory::Create(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_)
{
#if true
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	if (object_manager == nullptr) return nullptr;
	std::shared_ptr<ObjectBase> obj = std::make_shared<SampleStage>(position_, model_handle_, vertex_shader_handle_, pixel_shader_handle_, shadow_vs_handle_);
	object_manager->Register(obj);
	return obj;
#endif
	return nullptr;
}

std::shared_ptr<SampleCamera> ObjectFactory::CreateSampleCamera(Vector3 position_)
{
#if true
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	if (object_manager == nullptr) return nullptr;
	std::shared_ptr<SampleCamera> obj = std::make_shared<SampleCamera>(position_);
	object_manager->Register(obj);
	return obj;
#endif
	return nullptr;
}

std::shared_ptr<SampleCharacter> ObjectFactory::CreateCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_)
{
#if true
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	if (object_manager == nullptr) return nullptr;
	std::shared_ptr<SampleCharacter> obj = std::make_shared<SampleCharacter>(position_, scale_, model_handle_, vertex_shader_handle_, pixel_shader_handle_, shadow_vs_handle_, idle_anim_handle_, sample_camera_);
	object_manager->Register(obj);
	return obj;
#endif
	return nullptr;
}

std::shared_ptr<CameraBase> ObjectFactory::CreateCameraTPS()
{
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	if (!object_manager) { return nullptr; }

	auto obj = std::make_shared<CameraTPS>();

	obj->SetTransform(std::make_shared<Transform>(Vector3(0.0f, 12.0f, 0.0f)));

	object_manager->Register(obj);
	return obj;
}


std::shared_ptr<Player> ObjectFactory::CreatePlayer(Vector3 position_, Vector3 rotation_, std::shared_ptr <PlayerEventNotifier> notifier_, std::shared_ptr<CameraManager> camera_manager_)
{
	auto object_manager = objectManager.lock();
	auto assets_manager = assetsManager.lock();
	auto collider_interface = colliderRegisterInterface.lock();
	if (!object_manager || !assets_manager || !collider_interface) { return nullptr; }

	auto obj = std::make_shared<PlayerSaber>(camera_manager_);
	obj->SetComponent(std::make_shared<Transform>(position_, Vector3(0.1f, 0.1f, 0.1f), rotation_));
	obj->SetComponent(std::make_shared<Renderer>(
		assets_manager->GetModel(MKind::Player)->Handle,
		assets_manager->GetVertexShader(VSKind::SkinnedMesh)->Handle,
		assets_manager->GetPixelShader(PSKind::Phong)->Handle,
		assets_manager->GetVertexShader(VSKind::SkinnedMeshShadow)->Handle
	));
	obj->SetComponent(std::make_shared<Animator>(assets_manager->GetAnimation(MKind::Player)));
	obj->SetComponent(std::make_shared<PlayerStatus>(obj, notifier_,shared_from_this()));

	obj->SetColliderRegisterInterface(collider_interface);

	object_manager->Register(obj);
	return obj;
}

std::shared_ptr<Enemy> ObjectFactory::CreateEnemy(Vector3 position_, Vector3 rotation_)
{
	auto object_manager = objectManager.lock();
	auto assets_manager = assetsManager.lock();
	auto collider_interface = colliderRegisterInterface.lock();
	if (!object_manager || !assets_manager || !collider_interface) { return nullptr; }

	auto obj = std::make_shared<Enemy>();
	obj->SetComponent(std::make_shared<Transform>(position_, Vector3(0.18f, 0.18f, 0.18f), rotation_));
	obj->SetComponent(std::make_shared<Renderer>(
		assets_manager->GetModel(MKind::Enemy)->Handle,
		assets_manager->GetVertexShader(VSKind::SkinnedMesh)->Handle,
		assets_manager->GetPixelShader(PSKind::Phong)->Handle,
		assets_manager->GetVertexShader(VSKind::SkinnedMeshShadow)->Handle
	));
	obj->SetComponent(std::make_shared<Animator>(assets_manager->GetAnimation(MKind::Enemy)));
	obj->SetComponent(std::make_shared<EnemyStatus>(obj));
	obj->SetColliderRegisterInterface(collider_interface);

	object_manager->Register(obj);
	return obj;
}

std::shared_ptr<ObjectBase> ObjectFactory::CreateStage()
{
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	auto assets_manager = assetsManager.lock();
	if (!object_manager || !assets_manager) { return nullptr; }

	std::shared_ptr<ObjectBase> obj = std::make_shared<SampleStage>(
		Vector3(),
		assets_manager->GetModel(MKind::Stage)->Handle,
		assets_manager->GetVertexShader(VSKind::Rigidbody)->Handle,
		assets_manager->GetPixelShader(PSKind::Phong)->Handle,
		assets_manager->GetVertexShader(VSKind::RigidbodyShadow)->Handle);
	object_manager->Register(obj);
	return obj;
}

std::shared_ptr<ObjectBase> ObjectFactory::CreateSlashEffect(Vector3 position_, float size_, float rotation_)
{
	std::shared_ptr<ObjectManager> object_manager = objectManager.lock();
	auto assets_manager = assetsManager.lock();
	if (!object_manager || !assets_manager) { return nullptr; }

	std::shared_ptr<EffectBase> obj = std::make_shared<FlashEffect>(
		assets_manager->GetEffect(EKind::HitSlash),
		position_,
		Vector3(size_, rotation_, 0.0f));

	if (isJustAvoidTime)
	{
		obj->SetLocalTimeScale(GameScene::JustAvoidLocalTimeScale);
	}

	object_manager->Register(obj);
	return obj;
}