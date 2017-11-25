#pragma once

#include <map>

#include "../../Utility/Header/TSingleton.h"

#include "Asset.h"

LK_SPACE_BEGIN

class AssetManager
	: public TSingleton<AssetManager>
{
private:
	const std::string prefix = "Assets/";

	std::map<std::string, Asset*> assets;
public:
	explicit AssetManager();
	~AssetManager();

	template <typename T>
	inline T* Load(const std::string& path)
	{
		for (auto it : assets)
		{
			if (it.first == path)
			{
				return dynamic_cast<T*>(it.second);
			}
		}

		Asset* newAsset = new T(prefix + path);
		assets.insert(std::make_pair(path, newAsset));

		return dynamic_cast<T*>(newAsset);
	}
};

LK_SPACE_END