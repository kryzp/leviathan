#include <lev/graphics/render_target.h>
#include <backend/renderer.h>

using namespace lv;

Ref<RenderTarget> RenderTarget::create(unsigned width, unsigned height)
{
	Attachments attachment = {
        Pair(TextureFormatInfo(TEX_FMT_RGBA, TEX_I_FMT_RGBA32F, TEX_TYPE_UNSIGNED_BYTE), TextureSampler::pixel())
    };

	return create(width, height, attachment);
}

Ref<RenderTarget> RenderTarget::create(unsigned width, unsigned height, const Attachments& attachments)
{
	LEV_ASSERT(attachments.begin(), "Attachments must not be null");
	LEV_ASSERT(attachments.size() > 0, "Attachment count must be higher than 0");
	LEV_ASSERT(width > 0 && height > 0, "Width and Height must be higher than 0");

	int depthstencilcount = 0;

	for (int i = 0; i < attachments.size(); i++)
	{
		if (attachments[i].first.format == TEX_FMT_DEPTH_STENCIL)
			depthstencilcount++;

		LEV_ASSERT(depthstencilcount < 2, "There must not be 2 or more depth stencil attachments");
	}

	return bknd::Renderer::inst()->create_framebuffer(width, height, attachments);
}

void RenderTarget::unbind()
{
	bknd::Renderer::inst()->unbind_framebuffer();
}

Ref<Texture> RenderTarget::attachment(int i) const
{
	return attachments()[i];
}
