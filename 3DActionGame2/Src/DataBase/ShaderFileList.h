#pragma once
#include <utility>
#include "DataKind.h"

// ファイル名とかの管理はもっといいやり方を模索したい
// csvからの読み込みやVBAを使って生成するなど
struct ShaderFileList
{
	using VertexShaderList = std::pair<VSKind, const char*>;

	VertexShaderList VertexShaderFiles[4] = {
		std::make_pair(VSKind::Rigidbody, "Res/Shader/Test/TestVertex07.vso"),
		std::make_pair(VSKind::RigidbodyShadow, "Res/Shader/Test/TestShadowMapVS01.vso"),
		std::make_pair(VSKind::SkinnedMesh, "Res/Shader/Test/TestVertex08.vso"),
		std::make_pair(VSKind::SkinnedMeshShadow, "Res/Shader/Test/TestShadowMapVS00.vso")
	};

	using PixelShaderList = std::pair<PSKind, const char*>;

	PixelShaderList PixelShaderFiles[3] = {
		std::make_pair(PSKind::Phong, "Res/Shader/Test/TestPixel05.pso"),
		std::make_pair(PSKind::ShadowMap, "Res/Shader/Test/TestShadowMapPS00.pso"),
		std::make_pair(PSKind::PostEffect, "Res/Shader/Test/RadialBlurPS.pso")
	};
};
