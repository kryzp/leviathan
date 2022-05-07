#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/pair.h>
#include <lev/graphics/texture.h>
#include <lev/math/colour.h>

namespace lev
{
	class RenderTarget : public NonCopyable, public NonMovable
	{
	public:
		using Attachments = Vector<Pair<TextureFormatInfo, TextureSampler>>;

		virtual ~RenderTarget() = default;

		static Ref<RenderTarget> create(unsigned width, unsigned height);
		static Ref<RenderTarget> create(unsigned width, unsigned height, const Attachments& attachments);

		static void unbind();

		virtual void clear(const Colour& colour = Colour::empty()) = 0;
		virtual const Vector<Ref<Texture>>& attachments() const = 0;
		virtual int attachment_count() const = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;

		Ref<Texture> attachment(int i) const;
	};
}
