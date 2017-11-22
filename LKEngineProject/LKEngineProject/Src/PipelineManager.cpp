#include "PipelineManager.h"

#include "../Vulkan/Header/VulkanPipeline.h"
#include "../Vulkan/Header/VulkanShaderModule.h"
#include "../Vulkan/Header/VulkanDescriptorSetLayout.h"

USING_LK_SPACE

PipelineManager::PipelineManager()
{
}

PipelineManager::~PipelineManager()
{
	ReleaseAll();
}

void PipelineManager::AddPipeline(const std::string & key, Vulkan::VulkanPipeline * pipeline)
{
	pipelines.insert(std::make_pair(key, pipeline));
}

Vulkan::VulkanPipeline * PipelineManager::CreateGfxPipeline(const std::string& key, Vulkan::VulkanDescriptorSetLayout* setLayout, Vulkan::VulkanShaderModule* vertShader, Vulkan::VulkanShaderModule* fragShader)
{
	Vulkan::VulkanPipeline* gfxPipeline = new Vulkan::VulkanGraphicsPipeline(vertShader, fragShader, setLayout);
	AddPipeline(key, gfxPipeline);

	return gfxPipeline;
}

void PipelineManager::ReleasePipeline(const std::string & key)
{
	SAFE_DELETE(pipelines.at(key));
}

void PipelineManager::ReleaseAll()
{
	for (auto pipeline : pipelines)
	{
		SAFE_DELETE(pipeline.second);
	}
	pipelines.clear();
}

Vulkan::VulkanPipeline * PipelineManager::GetPipeline(const std::string & key) const
{
	return pipelines.at(key);
}


