#include <lev/graphics/framebuffer.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

Ref<Framebuffer> Framebuffer::create(int width, int height)
{
	// todo: i dont think this needs explaining
	static const TextureFormat fmt = TextureFormat::RGBA;
	return create(width, height, &fmt, 1);
}

Ref<Framebuffer> Framebuffer::create(int width, int height, const TextureFormat* attachments, int attachment_count)
{
	LEV_ASSERT(attachments);
	LEV_ASSERT(attachment_count > 0);
	LEV_ASSERT(width > 0 && height > 0);

	// todo: do a check to make sure there isn't more than one depth/stencil texture

	return Renderer::create_framebuffer({
		.width = width,
		.height = height,
		.attachments = attachments,
		.attachment_count = attachment_count
	});
}

const Ref<Texture>& Framebuffer::attachment(int i) const
{
	return attachments()[i];
}
