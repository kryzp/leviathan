#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/material.h>
#include <lev/graphics/framebuffer.h>

namespace lev
{
	struct RenderPass
	{
		Ref<gfx::Mesh> mesh;
		Ref<gfx::Material> material;
	};

	namespace Renderer
	{
		bool init();
		void destroy();

		void render(const RenderPass& pass);
		
		void before_render();
		void after_render();

		void clear(float r, float g, float b, float a = 1.0f);

		Ref<gfx::Texture> create_texture(const gfx::TextureData& data);
		Ref<gfx::Shader> create_shader(const gfx::ShaderData& data);
		Ref<gfx::Framebuffer> create_framebuffer();
		Ref<gfx::Mesh> create_mesh();
	}
}
