#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/graphics/texture.h>
#include <lev/math/colour.h>

namespace lev
{
	struct TextureAttachment
	{
		TextureFormat format;
		TextureType type;

		TextureAttachment(TextureFormat format, TextureType type)
			: format(format)
			, type(type)
		{
		}
	};

	struct FramebufferData
	{
		int width;
		int height;
		const TextureAttachment* attachments;
		int attachment_count;
	};

	class Framebuffer : public NonCopyable, public NonMovable
	{
	public:
		virtual ~Framebuffer() = default;

		static Ref<Framebuffer> create(int width, int height);
		static Ref<Framebuffer> create(int width, int height, const TextureAttachment* attachments, int attachment_count);

		static void unbind();

		virtual void clear(const Colour& colour = Colour::empty()) = 0;
		virtual const Vector<Ref<Texture>>& attachments() const = 0;
		virtual int attachment_count() const = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;

		const Ref<Texture>& attachment(int i) const;
	};
}
