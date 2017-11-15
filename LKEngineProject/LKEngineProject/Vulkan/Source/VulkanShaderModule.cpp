#include "../Header/VulkanShaderModule.h"

#include "../../Utility/Header/Macro.h"
#include <fstream>
#include <filesystem>

using namespace LKEngine::Vulkan;

VulkanShaderModule::VulkanShaderModule(VulkanDevice * device)
	: VulkanDeviceChild(device),
	shaderModule(VK_NULL_HANDLE)
{ }

VulkanShaderModule::~VulkanShaderModule()
{ }

void VulkanShaderModule::Init(const ShaderType type, const std::string & shaderPath)
{
	std::string compilerPath = "ThirdParty\\glslangValidator.exe";
	std::string command = compilerPath + " -V " + shaderPath;
	system(command.c_str());

	auto ExtractFileName = [](const std::string_view& fullPath)
	{
		const size_t lastSlashIndex = fullPath.find_last_of("/\\");
		return fullPath.substr(lastSlashIndex + 1);
	};
	auto ReplaceString = [](std::string subject, const std::string_view& search, const std::string_view& replace)
	{ 
		size_t pos = 0; 
		while ((pos = subject.find(search, pos)) != std::string::npos) 
		{ 
			subject.replace(pos, search.length(), replace); 
			pos += replace.length(); 
		} 
		return subject; 
	};
	auto RenameAndMoveShaderFile = [=](const std::string& format,const std::string& newFilename) {
		namespace fs = std::experimental::filesystem;

		if (!fs::exists("CompileShader/"))
		{
			fs::create_directory("CompileShader");
		}

		std::string oldFile = format;
		std::string newFile = "CompileShader/" + newFilename + ".spv";
		if (fs::exists(oldFile))
		{
			fs::rename(oldFile, newFile);
		}
		else
		{
			throw std::exception("Shader 컴파일 실패");
		}

		return newFile;
	};

	std::string shaderFilename = std::string(ExtractFileName(shaderPath));
	std::string shaderDirectory = ReplaceString(shaderPath, shaderFilename , "");
	switch (type)
	{
	case ShaderType::VERTEX:
		shaderFilename = RenameAndMoveShaderFile("vert.spv",shaderFilename);
		break;
	case ShaderType::FRAGMENT:
		shaderFilename = RenameAndMoveShaderFile("frag.spv", shaderFilename);
		break;
	}

	Init(type, ReadCompiledShader(shaderFilename));
}

void VulkanShaderModule::Init(const ShaderType type, const std::vector<char>& compileCode)
{
	shaderType = type;

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = compileCode.size();

	std::vector<uint32_t> codeAligned(compileCode.size() / sizeof(uint32_t) + 1);
	memcpy(codeAligned.data(), compileCode.data(), compileCode.size());

	createInfo.pCode = codeAligned.data();
	
	VkResult result = vkCreateShaderModule(*(*device), &createInfo, nullptr, &shaderModule);
	Check_Throw(result != VK_SUCCESS, "Can't Create ShaderModule!");
}

void VulkanShaderModule::Shutdown()
{
	vkDestroyShaderModule(*(*device), shaderModule, nullptr);
}

VkShaderModule VulkanShaderModule::GetHandle() const
{
	return shaderModule;
}

VulkanShaderModule::ShaderType VulkanShaderModule::GetShaderType() const
{
	return shaderType;
}

std::vector<char> VulkanShaderModule::ReadCompiledShader(const std::string & file)
{
	//ios::ate : 파일을 열때 파일 커서를 파일 끝에 두고 부터 열음
	std::ifstream shaderStream(file, std::ios::ate | std::ios::binary);

	if (!shaderStream.is_open())
	{
		throw std::runtime_error("쉐이더 파일 열기 실패");
	}

	size_t fileSize = (size_t)shaderStream.tellg();
	std::vector<char> buffer(fileSize);

	shaderStream.seekg(0);
	shaderStream.read(buffer.data(), fileSize);
	shaderStream.close();

	return buffer;
}
