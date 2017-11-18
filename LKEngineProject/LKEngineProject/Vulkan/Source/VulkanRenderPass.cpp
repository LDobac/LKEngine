#include "../Header/VulkanRenderPass.h"

#include "../../Utility/Header/Macro.h"
#include "../Header/VulkanSwapchain.h"


using namespace LKEngine::Vulkan;

VulkanRenderPass::VulkanRenderPass(VulkanDevice * device)
	:VulkanDeviceChild(device),
	renderPass(VK_NULL_HANDLE)
{ }

LKEngine::Vulkan::VulkanRenderPass::~VulkanRenderPass()
{ }

void VulkanRenderPass::Init(VulkanSwapchain * swapchain)
{
	Console_Log("렌더 패스 생성 시작");

	//컬러 버퍼 설명
	VkAttachmentDescription colorAttachment = {};
	//포맷은 스왑 체인 포맷과 일치 해야 함
	colorAttachment.format = swapchain->GetFormat();
	//MEMO : 차후에 멀티 샘플링을 사용시 설정 할 수 있음, 멀티 샘플링을 사용하지 않으므로 1 샘플 지정
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	//컬러 및 깊이 버퍼 등에 사용되는 데이터 플래그
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//스텐실 버퍼에 사용되는 데이터 플래그
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	/*
	* 일반적인 레이아웃 중 일부
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : 색상 첨부로 사용되는 이미지일경우
	* VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : 스왑 체인에 사용하는 이미지 일경우
	* VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : 메모리 복사 작업으로 사용되는 이미지 일 경우
	*
	* initalLayout은 렌더 패스가 시작되기 전 이미지가 가질 레이아웃, finalLayout은 렌더링 패스가 끝나면 전환되는 레이아웃
	* VK_IMAGE_LAYOUT_UNDEFINED를 사용하면 이전의 레이아웃을 신경 쓰지 않음을 의미
	* 렌더링 후에 스왑 체인으로 프레젠테이션을 해야 하므로 다음 플래그 지정
	*/
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//깊이 디스크립션
	VkFormat depthFormat = device->FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	VkAttachmentDescription depthAttachment = {};
	//포맷은 깊이 버퍼 포맷과 일치해야함
	depthAttachment.format = depthFormat;
	//MEMO : 차후에 멀티 샘플링을 사용시 설정 할 수 있음, 멀티 샘플링을 사용하지 않으므로 1 샘플 지정
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	/*
	* VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VkFramebuffer에 부착되는 깊이/스텐실 버퍼에 사용됨
	* 이 레이아웃은 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT 비트가 활성화 된 이미지의 서브 리소스에만 유효
	*/
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//컬러 디스크립션 참조 설정
	VkAttachmentReference colorAttachmentRef = {};
	/*
	* 첨부 파일 디스크립터 배열에서 인덱스를 기준으로 참조할 첨부 파일 지정
	* 컬러 디스크립터의 첨부파일은 인덱스가 0
	*/
	colorAttachmentRef.attachment = 0;
	//이 참조를 사용하는 서브 패스 중에 첨부 파일이 가져야 하는 레이아웃 지정
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//깊이 설명 참조 설정
	VkAttachmentReference depthAttachmentRef = {};
	//첨부 파일 디스크립터 배열에서의 첨부 파일은 인덱스가 1
	depthAttachmentRef.attachment = 1;
	//이 참조를 사용하는 서브 패스중 첨부 파일이 가져야 하는 레이아웃 지정
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//서브 패스 설명
	VkSubpassDescription subpass = {};
	//컴퓨트 서브 패스일 수도 있으므로, 그래픽 서브 패스라는걸 명시
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	/*
	* pInputAttachmnts : 쉐이더에서 읽은 파일 첨부
	* pResolveAttachments 멀티 샘플링 컬러 첨부에 사용되는 첨부 파일
	* pDepthStencilAttachment 깊이/스텐슬 데이터 첨부 파일
	* pPreserveAttachments : 이 서브 패스에서 사용하지 않지만 데이터를 보존해야 하는 첨부 파일
	*/
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	//서브 패스 종속성
	VkSubpassDependency dependency = {};
	/*
	* 종속성과 서브 패스의 인덱스 입력
	* dstSubpass 가 srcSubpass보다 작으면 순환 참조가 발생한다.
	*/
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	//작업을 기다려야 하는 곳이 색상 부착 단계이므로 해당 플래그 지정
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//위에서 생성한 정보를 바탕으로 렌더 패스 생성
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
	Check_Throw(result != VK_SUCCESS, "렌더 패스 생성 실패!");
	
	Console_Log("렌더 패스 생성 성공!");
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
	info.clearValueCount = clearColors.size();
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
