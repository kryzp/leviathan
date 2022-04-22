#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/pair.h>
#include <lev/graphics/texture.h>
#include <lev/math/colour.h>

namespace lev
{
	using FramebufferAttachments = Vector<Pair<TextureData, TextureSampler>>;

	struct FramebufferData
	{
        u32 width;
        u32 height;
		FramebufferAttachments attachments;
	};

	class Framebuffer : public NonCopyable, public NonMovable
	{
	public:
		virtual ~Framebuffer() = default;

		static Ref<Framebuffer> create(u32 width, u32 height);
		static Ref<Framebuffer> create(const FramebufferAttachments& attachments);

		static void unbind();

		virtual void clear(const Colour& colour = Colour::empty()) = 0;
		virtual const Vector<Ref<Texture>>& attachments() const = 0;
		virtual int attachment_count() const = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;

		const Ref<Texture>& attachment(int i) const;
	};
}
