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
		const Mesh* mesh;
		const Framebuffer* target;
		BlendMode blend;
		Compare stencil;
		u8 depth;
		RectI viewport;
		RectI scissor;
		int instance_count;
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

		virtual Texture* create_texture(const TextureData& data) = 0;
		virtual ArrayTexture* create_array_texture(const TextureData& data, u32 depth) = 0;
		virtual Shader* create_shader(const ShaderData& data) = 0;
		virtual ShaderBuffer* create_shader_buffer(u64 size) = 0;
		virtual Framebuffer* create_framebuffer(const FramebufferData& data) = 0;
		virtual Mesh* create_mesh() = 0;

		virtual void unbind_texture() = 0;
		virtual void unbind_array_texture() = 0;
		virtual void unbind_texture_image() = 0;
		virtual void unbind_shader() = 0;
		virtual void unbind_shader_buffer() = 0;
		virtual void unbind_framebuffer() = 0;
	};
}
