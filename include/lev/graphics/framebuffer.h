#pragma once

#include <lev/core/util.h>
#include <lev/core/non_copyable.h>
#include <lev/core/non_movable.h>
#include <lev/containers/vector.h>
#include <lev/graphics/texture.h>
#include <lev/math/colour.h>

namespace lev::gfx
{
	struct FramebufferData
	{
		int width;
		int height;
		const TextureFormat* attachments;
		int attachment_count;
	};

	class Framebuffer : public NonCopyable, public NonMovable
	{
	public:
		static Ref<Framebuffer> create(int width, int height);
		static Ref<Framebuffer> create(int width, int height, const TextureFormat* attachments, int attachment_count);

		virtual void clear(const Colour& colour = Colour::empty()) = 0;
		virtual const Vector<Ref<Texture>>& attachments() const = 0;
		virtual int attachment_count() const = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;

		const Ref<Texture>& attachment(int i) const;
	};
}
