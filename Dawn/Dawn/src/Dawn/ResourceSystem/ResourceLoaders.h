#pragma once
#include "Resources.h"

namespace Dawn
{
	class ResourceSystem;
	struct FileMetaData;

	MeshHandle RS_LoadStaticMesh(ResourceSystem* InFS, std::string& InWorkspacePath, FileMetaData* InMetaData);
	ShaderHandle RS_LoadShader(ResourceSystem* InFS, std::string& InWorkspacePath, FileMetaData* InMetaData);
	ImageHandle RS_LoadImage(ResourceSystem* InFS, std::string& InWorkspacePath, FileMetaData* InMetaData);
}