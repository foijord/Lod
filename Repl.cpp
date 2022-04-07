#include <boost/asio.hpp>

#include <ExamineMode.h>
#include <ShaderReload.h>

#include <Innovator/Nodes.h>
#include <Innovator/Window.h>
#include <Innovator/GroupNodes.h>
#include <Innovator/GliTextureImage.h>

#include <iostream>


int main(int, char* [])
{
	try {
		auto image = std::make_shared<GliTextureImage>("fullres.ktx");
		auto textureimage = std::make_shared<TextureImage>(image);

		auto texture = std::make_shared<Texture>(
			0,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			VK_FILTER_NEAREST,
			VK_SAMPLER_MIPMAP_MODE_NEAREST,
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

		auto render_command = std::make_shared<RenderCommand>();
		render_command->children = {
			textureimage,
			texture,

			std::make_shared<Shader>(VK_SHADER_STAGE_VERTEX_BIT, "shaders/vertex.glsl"),
			std::make_shared<Shader>(VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/fragment.glsl"),

			std::make_shared<TransformBuffer>(),
			std::make_shared<DescriptorSetLayoutBinding>(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),

			std::make_shared<GraphicsPipeline>(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
			std::make_shared<DrawCommand>(3u, 1u, 0u, 0u),
		};

		auto framebuffer_object = std::make_shared<Separator>();
		framebuffer_object->children = {
			std::make_shared<ColorAttachmentReference>(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
			std::make_shared<ColorAttachmentReference>(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
			std::make_shared<DepthStencilAttachmentReference>(2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL),

			std::make_shared<ColorAttachmentDescription>(VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT),
			std::make_shared<ColorAttachmentDescription>(VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT),
			std::make_shared<DepthStencilAttachmentDescription>(VK_FORMAT_D32_SFLOAT_S8_UINT, VK_SAMPLE_COUNT_1_BIT),

			std::make_shared<SubpassDescription>(VK_PIPELINE_BIND_POINT_GRAPHICS),
			std::make_shared<Renderpass>(),

			std::make_shared<PipelineColorBlendAttachmentState>(),
			std::make_shared<ClearValue>(VkClearValue{.color = { 1.0f, 1.0f, 1.0f, 0.0f } }),
			std::make_shared<ColorAttachment>(VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT),

			std::make_shared<PipelineColorBlendAttachmentState>(),
			std::make_shared<ClearValue>(VkClearValue{.color = { 1.0f, 1.0f, 1.0f, 0.0f } }),
			std::make_shared<ColorAttachment>(VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT),

			std::make_shared<ClearValue>(VkClearValue{.depthStencil = { 1.0f, 0 } }),
			std::make_shared<DepthStencilAttachment>(VK_FORMAT_D32_SFLOAT_S8_UINT, VK_SAMPLE_COUNT_1_BIT),

			std::make_shared<Framebuffer>(),

			render_command
		};

		boost::asio::io_context io_context;
		auto work_guard = boost::asio::make_work_guard(io_context);

		VkExtent2D extent{
			.width = 1920,
			.height = 1080,
		};

		ExamineMode mode;
		NativeWindow window(io_context, extent, framebuffer_object);
#ifdef DEBUG
		ShaderReload shader_reload(io_context, framebuffer_object);
#endif
		return window.show();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
