#ifndef __MODULEMESHES_H__
#define __MODULEMESHES_H__

#include "Module.h"

#include <string>

struct aiMesh;
class ResourceMesh;
class Resource;

class ModuleMeshes : public Module
{
public:
	ModuleMeshes(bool start_enabled = true);
	~ModuleMeshes() {}

	//Changed
	bool importRMesh(aiMesh* mesh, uint UID, std::vector<uint> rBonesUID, std::string& path);
	bool saveMesh(ResourceMesh* mesh, uint UID, std::string& newpath);
	
};

#endif //__MODULEMESHES_H__