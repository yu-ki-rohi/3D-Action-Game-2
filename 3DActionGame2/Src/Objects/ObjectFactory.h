#pragma once
#include <memory>

struct Vector3;

class ObjectBase;
class ObjectManager;
class AssetsManager;
class ColliderRegisterInterface;

class CameraManager;
class PlayerEventNotifier;

class SampleCharacter;
class SampleCamera;

class CameraBase;
class Player;
class Enemy;

class ObjectFactory
{
public:
	ObjectFactory(std::shared_ptr<ObjectManager> object_manager_, std::shared_ptr<AssetsManager> assets_manager_, std::shared_ptr<ColliderRegisterInterface> collider_register_interface_);

public:
	std::shared_ptr<ObjectBase> Create();
	std::shared_ptr<ObjectBase> Create(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_);

	std::shared_ptr<SampleCamera> CreateSampleCamera(Vector3 position_);
	std::shared_ptr<SampleCharacter> CreateCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_);
	
	std::shared_ptr<CameraBase> CreateCameraTPS();
	std::shared_ptr<Player> CreatePlayer(std::shared_ptr <PlayerEventNotifier> notifier_, std::shared_ptr<CameraManager> camera_manager_);
	std::shared_ptr<Enemy> CreateEnemy();
	std::shared_ptr<ObjectBase> CreateStage();

private:
	std::weak_ptr<ObjectManager> objectManager;
	std::weak_ptr<AssetsManager> assetsManager;
	std::weak_ptr<ColliderRegisterInterface> colliderRegisterInterface;
};