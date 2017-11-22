#pragma once

#include <map>

#include "../Utility/Header/TSingleton.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanPipeline;
	class VulkanShaderModule;
	class VulkanDescriptorSetLayout;
}

class PipelineManager
	: public TSingleton<PipelineManager>
{
private:
	std::map<std::string, Vulkan::VulkanPipeline*> pipelines;
public:
	explicit PipelineManager();
	~PipelineManager();

	void AddPipeline(const std::string& key, Vulkan::VulkanPipeline* pipeline);

	Vulkan::VulkanPipeline* CreateGfxPipeline(const std::string& key, Vulkan::VulkanDescriptorSetLayout* setLayout,Vulkan::VulkanShaderModule* vertShader, Vulkan::VulkanShaderModule* fragShader);

	void ReleasePipeline(const std::string& key);
	void ReleaseAll();

	Vulkan::VulkanPipeline* GetPipeline(const std::string& key) const;
};

LK_SPACE_END