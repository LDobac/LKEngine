#include "../Header/VulkanRenderPass.h"

#include "../Header/VulkanSwapchain.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanRenderPass::VulkanRenderPass(VulkanDevice * device)
	:VulkanDeviceChild(device),
	renderPass(VK_NULL_HANDLE)
{ }

LKEngine::Vulkan::VulkanRenderPass::~VulkanRenderPass()
{ }

void VulkanRenderPass::Init(VulkanSwapchain * swapchain)
{
	//�÷� ���� ����
	VkAttachmentDescription colorAttachment = {};
	//������ ���� ü�� ���˰� ��ġ �ؾ� ��
	colorAttachment.format = swapchain->GetFormat();
	//MEMO : ���Ŀ� ��Ƽ ���ø��� ���� ���� �� �� ����, ��Ƽ ���ø��� ������� �����Ƿ� 1 ���� ����
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	//�÷� �� ���� ���� � ���Ǵ� ������ �÷���
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//���ٽ� ���ۿ� ���Ǵ� ������ �÷���
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	/*
	* �Ϲ����� ���̾ƿ� �� �Ϻ�
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : ���� ÷�η� ���Ǵ� �̹����ϰ��
	* VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : ���� ü�ο� ����ϴ� �̹��� �ϰ��
	* VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : �޸� ���� �۾����� ���Ǵ� �̹��� �� ���
	*
	* initalLayout�� ���� �н��� ���۵Ǳ� �� �̹����� ���� ���̾ƿ�, finalLayout�� ������ �н��� ������ ��ȯ�Ǵ� ���̾ƿ�
	* VK_IMAGE_LAYOUT_UNDEFINED�� ����ϸ� ������ ���̾ƿ��� �Ű� ���� ������ �ǹ�
	* ������ �Ŀ� ���� ü������ ���������̼��� �ؾ� �ϹǷ� ���� �÷��� ����
	*/
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//���� ��ũ����
	VkFormat depthFormat = device->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	VkAttachmentDescription depthAttachment = {};
	//������ ���� ���� ���˰� ��ġ�ؾ���
	depthAttachment.format = depthFormat;
	//MEMO : ���Ŀ� ��Ƽ ���ø��� ���� ���� �� �� ����, ��Ƽ ���ø��� ������� �����Ƿ� 1 ���� ����
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	/*
	* VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VkFramebuffer�� �����Ǵ� ����/���ٽ� ���ۿ� ����
	* �� ���̾ƿ��� VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ��Ʈ�� Ȱ��ȭ �� �̹����� ���� ���ҽ����� ��ȿ
	*/
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//�÷� ��ũ���� ���� ����
	VkAttachmentReference colorAttachmentRef = {};
	/*
	* ÷�� ���� ��ũ���� �迭���� �ε����� �������� ������ ÷�� ���� ����
	* �÷� ��ũ������ ÷�������� �ε����� 0
	*/
	colorAttachmentRef.attachment = 0;
	//�� ������ ����ϴ� ���� �н� �߿� ÷�� ������ ������ �ϴ� ���̾ƿ� ����
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//���� ���� ���� ����
	VkAttachmentReference depthAttachmentRef = {};
	//÷�� ���� ��ũ���� �迭������ ÷�� ������ �ε����� 1
	depthAttachmentRef.attachment = 1;
	//�� ������ ����ϴ� ���� �н��� ÷�� ������ ������ �ϴ� ���̾ƿ� ����
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//���� �н� ����
	VkSubpassDescription subpass = {};
	//��ǻƮ ���� �н��� ���� �����Ƿ�, �׷��� ���� �н���°� ���
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	/*
	* pInputAttachmnts : ���̴����� ���� ���� ÷��
	* pResolveAttachments ��Ƽ ���ø� �÷� ÷�ο� ���Ǵ� ÷�� ����
	* pDepthStencilAttachment ����/���ٽ� ������ ÷�� ����
	* pPreserveAttachments : �� ���� �н����� ������� ������ �����͸� �����ؾ� �ϴ� ÷�� ����
	*/
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	//���� �н� ���Ӽ�
	VkSubpassDependency dependency = {};
	/*
	* ���Ӽ��� ���� �н��� �ε��� �Է�
	* dstSubpass �� srcSubpass���� ������ ��ȯ ������ �߻��Ѵ�.
	*/
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	//�۾��� ��ٷ��� �ϴ� ���� ���� ���� �ܰ��̹Ƿ� �ش� �÷��� ����
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//������ ������ ������ �������� ���� �н� ����
	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkResult result = vkCreateRenderPass(device->GetHandle(), &renderPassInfo, nullptr, &renderPass);
	Check_Throw(result != VK_SUCCESS, "���� �н� ���� ����!");
}

void VulkanRenderPass::Shutdown()
{
	vkDestroyRenderPass(device->GetHandle(), renderPass, nullptr);
}

void VulkanRenderPass::Begin(std::vector<VkClearValue> clearColors, const VkFramebuffer& frameBuffer, VkExtent2D extent, const VkCommandBuffer& cmdBuffer)
{
	VkRenderPassBeginInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = renderPass;
	info.framebuffer = frameBuffer;
	info.renderArea.offset = { 0, 0 };
	info.renderArea.extent = extent;
	info.clearValueCount = static_cast<uint32_t>(clearColors.size());
	info.pClearValues = clearColors.data();

	vkCmdBeginRenderPass(cmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderPass::End(const VkCommandBuffer & cmdBuffer)
{
	vkCmdEndRenderPass(cmdBuffer);
}

VkRenderPass VulkanRenderPass::GetHandle() const
{
	return renderPass;
}
