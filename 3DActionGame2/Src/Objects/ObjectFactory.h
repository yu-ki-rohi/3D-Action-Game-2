#pragma once
#include <memory>

struct Vector3;

class ObjectBase;
class ObjectManager;
class AssetsManager;
class IColliderRegister;

class CameraManager;
class PlayerEventNotifier;


class CameraBase;
class Player;
class Enemy;

namespace EnemyAI
{
	class IEnemyDirectiveReader;
}

class ObjectFactory : public std::enable_shared_from_this<ObjectFactory>
{
public:
	ObjectFactory(std::shared_ptr<ObjectManager> object_manager_, std::shared_ptr<AssetsManager> assets_manager_, std::shared_ptr<IColliderRegister> collider_register_interface_);

public:
	void SetIsJustAvoidTime(bool is_just_avoid_time_);

public:
	std::shared_ptr<CameraBase> CreateCameraTPS();
	std::shared_ptr<Player> CreatePlayer(Vector3 position_, Vector3 rotation_, std::shared_ptr <PlayerEventNotifier> notifier_, std::shared_ptr<CameraManager> camera_manager_);
	std::shared_ptr<Enemy> CreateEnemy(Vector3 position_, Vector3 rotation_, unsigned char id_, std::shared_ptr<EnemyAI::IEnemyDirectiveReader> directive_);
	std::shared_ptr<ObjectBase> CreateStage();

	std::shared_ptr<ObjectBase> CreateSlashEffect(Vector3 position_, float size_, float rotation_);

private:
	void CommonProcessWhenCreate(std::shared_ptr<ObjectBase> obj_, std::shared_ptr<ObjectManager> object_manager_);

private:
	std::weak_ptr<ObjectManager> objectManager;
	std::weak_ptr<AssetsManager> assetsManager;
	std::weak_ptr<IColliderRegister> colliderRegisterInterface;

private:
	// 必要に応じて生成時に固有タイムスケールを変更するため
	// 一応ここに置いているが、他の場所において参照する方が良い気がする
	// TODO: 他クラスで管理
	bool isJustAvoidTime;
};