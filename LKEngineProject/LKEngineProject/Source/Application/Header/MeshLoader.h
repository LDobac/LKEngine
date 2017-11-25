#pragma once

#include <string>

#include "../../Utility/Header/Macro.h"

#include "../Header/VertexInformation.h"

LK_SPACE_BEGIN

class MeshLoader
{
public:
	static MeshData* LoadOBJ(const std::string& path);
};

LK_SPACE_END