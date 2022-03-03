#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/material.h>
#include <lev/graphics/framebuffer.h>

namespace Lev
{
	enum class RendererType
	{
		None = -1,
		OpenGL,
		Max
	};

	struct RenderPass
	{
		// todo: temp, this stuff will be in the mesh
		float* vertices;
		int vertex_count;

		u32* indices;
		int index_count;

		// todo
		Ref<Mesh> mesh;
		Ref<Material> material;
	};

	namespace Renderer
	{
		bool init();
		void destroy();

		void render(const RenderPass& pass);
		
		void before_render();
		void after_render();

		void clear(float r, float g, float b, float a = 1.0f);

		Ref<Texture> create_texture(const TextureData& data);
		Ref<Shader> create_shader(const ShaderData& data);
		Ref<Framebuffer> create_framebuffer();
		Ref<Mesh> create_mesh();

		RendererType renderer_type();
	}
}
