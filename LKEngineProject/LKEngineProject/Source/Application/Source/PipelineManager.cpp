#include "../Header/PipelineManager.h"

#include "../../Renderer/Header/VulkanPipeline.h"
#include "../../Renderer/Header/VulkanShaderModule.h"
#include "../../Renderer/Header/VulkanDescriptorSetLayout.h"
#include "../../Renderer/Header/VulkanShaderModule.h"

#include "../Header/EntityPool.h"

USING_LK_SPACE

PipelineManager::PipelineManager()
{
	{
		auto vertShader = new Vulkan::VulkanShaderModule(Vulkan::VulkanShaderModule::ShaderType::VERTEX, "Shader/SimpleShader.vert");
		auto fragShader = new Vulkan::VulkanShaderModule(Vulkan::VulkanShaderModule::ShaderType::FRAGMENT, "Shader/SimpleShader.frag");
		CreateGfxPipeline("Default",
		{
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultCamera"),
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMesh"),
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial")
		},
			vertShader,
			fragShader);
	}
}

PipelineManager::~PipelineManager()
{
	ReleaseAll();
}

void PipelineManager::AddPipeline(const std::string & key, Vulkan::VulkanPipeline * pipeline)
{
	pipelines.insert(std::make_pair(key, pipeline));
}

Vulkan::VulkanPipeline * PipelineManager::CreateGfxPipeline(const std::string& key, const std::vector<Vulkan::VulkanDescriptorSetLayout*>& setLayouts, Vulkan::VulkanShaderModule* vertShader, Vulkan::VulkanShaderModule* fragShader)
{
	Vulkan::VulkanPipeline* gfxPipeline = new Vulkan::VulkanGraphicsPipeline(vertShader, fragShader, setLayouts);
	AddPipeline(key, gfxPipeline);

	return gfxPipeline;
}

void PipelineManager::RecreatePipelines()
{
	for (auto pipeline : pipelines)
	{
		pipeline.second->Recreate();
	}
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


