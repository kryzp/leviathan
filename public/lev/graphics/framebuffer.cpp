#include <lev/graphics/framebuffer.h>
#include <backend/renderer.h>

using namespace lev;

Ref<Framebuffer> Framebuffer::create(int width, int height)
{
	static const TextureAttachment attachment = TextureAttachment(TEXTURE_FORMAT_RGBA, TEXTURE_TYPE_UNSIGNED_BYTE);
	return create(width, height, &attachment, 1);
}

Ref<Framebuffer> Framebuffer::create(int width, int height, const TextureAttachment* attachments, int attachment_count)
{
	LEV_ASSERT(attachments, "Attachments must not be null");
	LEV_ASSERT(attachment_count > 0, "Attachment count must be higher than 0");
	LEV_ASSERT(width > 0 && height > 0, "Width and Height must be higher than 0");

	int depthstencilcount = 0;

	for (int i = 0; i < attachment_count; i++)
	{
		if (attachments[i].format == TEXTURE_FORMAT_DEPTH_STENCIL)
			depthstencilcount++;

		LEV_ASSERT(depthstencilcount < 2, "There must not be 2 or more depth stencil attachments");
	}

	return Renderer::inst()->create_framebuffer({
		.width = width,
		.height = height,
		.attachments = attachments,
		.attachment_count = attachment_count
	});
}

void Framebuffer::unbind()
{
	Renderer::inst()->unbind_framebuffer();
}

const Ref<Texture>& Framebuffer::attachment(int i) const
{
	return attachments()[i];
}
