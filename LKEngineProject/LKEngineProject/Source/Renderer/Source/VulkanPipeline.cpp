#include "../Header/VulkanPipeline.h"

#include "../Header/VulkanShaderModule.h"
#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanDescriptorSetLayout.h"
#include "../Header/VulkanRenderPass.h"

#include "../../Application/Header/VertexInformation.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanPipeline::VulkanPipeline()
	:VulkanDeviceChild(VulkanDevice::GetInstance()),
	pipeline(VK_NULL_HANDLE)
{
}

VulkanPipeline::~VulkanPipeline()
{
	ReleasePipeline();
}

void VulkanPipeline::Recreate()
{
	ReleasePipeline();
}

const VkPipeline & VulkanPipeline::GetHandle() const
{
	return pipeline;
}

const VkPipelineLayout & VulkanPipeline::GetLayout() const
{
	return pipelineLayout;
}

void VulkanPipeline::ReleasePipeline()
{
	if (pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(device->GetHandle(), pipelineLayout, nullptr);
		vkDestroyPipeline(device->GetHandle(), pipeline, nullptr);
	}
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanShaderModule * vertShader, VulkanShaderModule * fragShader, const std::vector<VulkanDescriptorSetLayout*>& descriptorSetLayouts)
{
	this->vertShader = vertShader;
	this->fragShader = fragShader;
	this->descriptorSetLayouts = descriptorSetLayouts;

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = ShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = ShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT,fragShader);
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = VertexInformation::BindingDescription();
	auto attributeDescriptions = VertexInformation::AttributeDescription();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = VertexInputInfo(bindingDescription,attributeDescriptions);

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = InputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	VulkanSwapchain* swapchain = VulkanDevice::GetInstance()->GetSwapchain();
	VkViewport viewport = {
		0.0f,
		0.0f,
		(float)swapchain->GetExtent().width,
		(float)swapchain->GetExtent().height,
		0.0f,
		1.0f
	};

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain->GetExtent();

	VkPipelineViewportStateCreateInfo viewportState = ViewportStateInfo(viewport, scissor);

	VkPipelineRasterizationStateCreateInfo rasterizer = RasterizerInfo(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);

	VkPipelineMultisampleStateCreateInfo multisampling = MultisamplingInfo();

	VkPipelineDepthStencilStateCreateInfo depthStencil = DepthStencilInfo();

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	VkPipelineColorBlendStateCreateInfo colorBlending = ColorBlendingInfo(colorBlendAttachment);

	CreatePipelineLayout();
	
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = VulkanDevice::GetInstance()->GetRenderPass()->GetHandle();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	Check_Throw(vkCreateGraphicsPipelines(device->GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS, "그래픽 파이프라인 생성 실패");
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
	SAFE_DELETE(vertShader);
	SAFE_DELETE(fragShader);
}

void VulkanGraphicsPipeline::Recreate()
{
	VulkanPipeline::Recreate();

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = ShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = ShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader);
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = VertexInformation::BindingDescription();
	auto attributeDescriptions = VertexInformation::AttributeDescription();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = VertexInputInfo(bindingDescription, attributeDescriptions);

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = InputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	VulkanSwapchain* swapchain = VulkanDevice::GetInstance()->GetSwapchain();
	VkViewport viewport = {
		0.0f,
		0.0f,
		(float)swapchain->GetExtent().width,
		(float)swapchain->GetExtent().height,
		0.0f,
		1.0f
	};

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain->GetExtent();

	VkPipelineViewportStateCreateInfo viewportState = ViewportStateInfo(viewport, scissor);

	VkPipelineRasterizationStateCreateInfo rasterizer = RasterizerInfo(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);

	VkPipelineMultisampleStateCreateInfo multisampling = MultisamplingInfo();

	VkPipelineDepthStencilStateCreateInfo depthStencil = DepthStencilInfo();

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	VkPipelineColorBlendStateCreateInfo colorBlending = ColorBlendingInfo(colorBlendAttachment);

	CreatePipelineLayout();

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = VulkanDevice::GetInstance()->GetRenderPass()->GetHandle();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	Check_Throw(vkCreateGraphicsPipelines(device->GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS, "그래픽 파이프라인 생성 실패");
}

void VulkanGraphicsPipeline::CreatePipelineLayout()
{
	VkDescriptorSetLayout* setLayouts = new VkDescriptorSetLayout[descriptorSetLayouts.size()];
	for (size_t i = 0; i < descriptorSetLayouts.size(); i++)
	{
		setLayouts[i] = descriptorSetLayouts[i]->GetHandle();
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = setLayouts;

	Check_Throw(vkCreatePipelineLayout(device->GetHandle(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS, "PipelineLayout 생성 실패");

	SAFE_ARR_DELETE(setLayouts);
}

VkPipelineShaderStageCreateInfo VulkanGraphicsPipeline::ShaderStageInfo(VkShaderStageFlagBits stage, const VulkanShaderModule * module)
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = module->GetHandle();
	shaderStageInfo.pName = "main";

	return shaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::VertexInputInfo(const VkVertexInputBindingDescription & bindingDescription, const std::vector<VkVertexInputAttributeDescription>& attributeDescription)
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = { };
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::InputAssemblyInfo(const VkPrimitiveTopology & topology)
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = topology;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	return inputAssembly;
}

VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::ViewportStateInfo(const VkViewport & viewport, const VkRect2D & scissor)
{
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	return viewportState;
}

VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipeline::RasterizerInfo(const VkCullModeFlags & cullmode, const VkFrontFace & frontface)
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = cullmode;
	rasterizer.frontFace = frontface;
	rasterizer.depthBiasEnable = VK_FALSE;

	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::MultisamplingInfo()
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	return multisampling;
}

VkPipelineDepthStencilStateCreateInfo VulkanGraphicsPipeline::DepthStencilInfo()
{
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	return depthStencil;
}

VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::ColorBlendingInfo(const VkPipelineColorBlendAttachmentState & attachment)
{
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &attachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	return colorBlending;
}
