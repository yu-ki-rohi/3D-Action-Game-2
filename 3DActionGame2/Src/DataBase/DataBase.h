#pragma once
#include <memory>
#include <vector>

struct AnimationFileList;
struct ModelFileList;
struct ShaderFileList;

class DataBase
{
public:
	std::shared_ptr<AnimationFileList> GetAnimationFileList();
	std::shared_ptr<ModelFileList> GetModelFileList();
	std::shared_ptr<ShaderFileList> GetShaderFileList();

private:



#pragma region ƒVƒ“ƒOƒ‹ƒgƒ““o˜^
public:
	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;
	DataBase(DataBase&&) = delete;
	DataBase& operator=(DataBase&&) = delete;

private:
	DataBase() = default;
	~DataBase() = default;
#pragma endregion

};