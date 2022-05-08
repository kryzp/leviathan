#pragma once

#include <lev/core/util.h>

#include <lev/graphics/render_pass.h>
#include <lev/graphics/blend.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/material.h>
#include <lev/graphics/render_target.h>
#include <lev/graphics/compare.h>

namespace lev::bknd
{
	struct RendererProperties
	{
		bool origin_bottom_left;
	};

	class Renderer : public NonCopyable, public NonMovable
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

        static Renderer* inst();

		virtual RendererProperties properties() = 0;

        virtual bool init() = 0;
        virtual void destroy() = 0;

        virtual void render(const RenderPass& pass) = 0;
        virtual void before_render() = 0;
        virtual void after_render() = 0;

        virtual void clear(const Colour& colour = Colour::empty()) = 0;

		virtual Ref<Texture> create_texture(u32 width, u32 height, const TextureFormatInfo& format_info) = 0;
		virtual Ref<ArrayTexture> create_array_texture(u32 width, u32 height, const TextureFormatInfo& format_info, u32 depth) = 0;
		virtual Ref<Shader> create_shader(const ShaderData& data) = 0;
		virtual Ref<ShaderBuffer> create_shader_buffer(u64 size) = 0;
		virtual Ref<RenderTarget> create_framebuffer(u32 width, u32 height, const RenderTarget::Attachments& attachments) = 0;
		virtual Ref<Mesh> create_mesh() = 0;

		virtual void unbind_texture() = 0;
		virtual void unbind_array_texture() = 0;
		virtual void unbind_texture_image() = 0;
		virtual void unbind_shader() = 0;
		virtual void unbind_shader_buffer() = 0;
		virtual void unbind_framebuffer() = 0;
	};
}
