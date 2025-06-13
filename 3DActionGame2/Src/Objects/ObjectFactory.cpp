#include "ObjectFactory.h"
#include "ObjectManager.h"
//#include "TestObject.h"
#include "SampleCamera.h"
#include "SampleStage.h"
#include "SampleCharacter.h"

ObjectFactory::ObjectFactory(std::shared_ptr<ObjectManager> object_manager_, std::shared_ptr<AssetsManager> assets_manager_) :
	objectManager(object_manager_),
	assetsManager(assets_manager_)
{

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

std::shared_ptr<SampleCamera> ObjectFactory::CreateCamera(Vector3 position_)
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
	obj->Start();
	return obj;
#endif
	return nullptr;
}