#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/graphics/blend.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/material.h>
#include <lev/graphics/framebuffer.h>

namespace lev
{
	struct RenderPass
	{
		BlendMode blend;
		Material material;
		Ref<Mesh> mesh;
		Ref<Framebuffer> target;
	};

	namespace Renderer
	{
		bool init();
		void destroy();

		void render(const RenderPass& pass);
		
		void before_render();
		void after_render();

		void clear(float r, float g, float b, float a = 0.0f);
		void clear(const Colour& colour = Colour::empty());

		Ref<Texture> create_texture(const TextureData& data);
		Ref<Shader> create_shader(const ShaderData& data);
		Ref<ShaderBuffer> create_shader_buffer(u64 size);
		Ref<Framebuffer> create_framebuffer(const FramebufferData& data);
		Ref<Mesh> create_mesh();

		void unbind_texture();
		void unbind_texture_image();
		void unbind_shader();
		void unbind_shader_buffer();
		void unbind_framebuffer();
	}
}
