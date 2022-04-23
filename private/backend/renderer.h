#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/graphics/blend.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/material.h>
#include <lev/graphics/framebuffer.h>
#include <lev/graphics/compare.h>

namespace lev
{
	struct RenderPass
	{
		Material material;
		BlendMode blend;
		Ref<Mesh> mesh;
		Ref<Framebuffer> target;
		Compare stencil;
		u8 depth;
		RectI viewport;
		RectI scissor;
	};

	class Renderer
	{
	public:
        static Renderer* inst();

        virtual bool init() = 0;
        virtual void destroy() = 0;

        virtual void render(const RenderPass& pass) = 0;
        virtual void before_render() = 0;
        virtual void after_render() = 0;

        virtual void clear(const Colour& colour = Colour::empty()) = 0;

		virtual Ref<Texture> create_texture(const TextureData& data) = 0;
		virtual Ref<Shader> create_shader(const ShaderData& data) = 0;
		virtual Ref<ShaderBuffer> create_shader_buffer(u64 size) = 0;
		virtual Ref<Framebuffer> create_framebuffer(const FramebufferData& data) = 0;
		virtual Ref<Mesh> create_mesh() = 0;

		virtual void unbind_texture() = 0;
		virtual void unbind_texture_image() = 0;
		virtual void unbind_shader() = 0;
		virtual void unbind_shader_buffer() = 0;
		virtual void unbind_framebuffer() = 0;
	};
}
