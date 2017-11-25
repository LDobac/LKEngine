#include "../Header/AssetManager.h"

USING_LK_SPACE

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	for (auto asset : assets)
	{
		SAFE_DELETE(asset.second);
	}
}
