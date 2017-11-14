#include "../Header/VulkanShaderModule.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanShaderModule::VulkanShaderModule(VulkanDevice * device)
	: VulkanDeviceChild(device),
	shaderModule(VK_NULL_HANDLE)
{ }

VulkanShaderModule::~VulkanShaderModule()
{ }

void VulkanShaderModule::Init(const std::string & shaderPath)
{
	std::string compilerPath = "../../ThirdParty/glslangValidator.exe";
	system((compilerPath + " " + shaderPath).c_str());
}

void VulkanShaderModule::Init(const std::vector<char> compileCode)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = compileCode.size();

	std::vector<uint32_t> codeAligned(compileCode.size() / sizeof(uint32_t) + 1);
	memcpy(codeAligned.data(), compileCode.data(), compileCode.size());

	createInfo.pCode = codeAligned.data();
	
	VkResult result = vkCreateShaderModule(device->GetRawDevice(), &createInfo, nullptr, &shaderModule);
	Check_Throw(result != VK_SUCCESS, "Can't Create ShaderModule!");
}

void VulkanShaderModule::Shutdown()
{
	vkDestroyShaderModule(device->GetRawDevice(), shaderModule, nullptr);
}

VkShaderModule LKEngine::Vulkan::VulkanShaderModule::GetHandle() const
{
	return shaderModule;
}
