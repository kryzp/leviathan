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
		gfx::BlendMode blend;
		Ref<gfx::Mesh> mesh;
		Ref<gfx::Material> material;
		Ref<gfx::Framebuffer> target;
	};

	namespace Renderer
	{
		bool init();
		void destroy();

		void render(const RenderPass& pass);
		
		void before_render();
		void after_render();

		void clear(float r, float g, float b, float a = 1.0f);
		void clear(const Colour& colour = Colour::BLACK);

		Ref<gfx::Texture> create_texture(const gfx::TextureData& data);
		Ref<gfx::Shader> create_shader(const gfx::ShaderData& data);
		Ref<gfx::Framebuffer> create_framebuffer(const gfx::FramebufferData& data);
		Ref<gfx::Mesh> create_mesh();
	}
}
