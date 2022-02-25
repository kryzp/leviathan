#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>

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
		float* vertices; // todo: super temporary! (maybe if i keep saying this i can trick myself into thinking i wont keep this till the end of time)
		int vertex_count;

		u32* indices;
		int index_count;
		
		// data on what to render...
	};

	namespace Graphics
	{
		bool init();
		void destroy();

		void render(const RenderPass& pass);
		
		void before_render();
		void after_render();

		void clear(float r, float g, float b, float a = 1.0f);

		Ref<Texture> create_texture(int width, int height, TextureFormat format);
		Ref<Shader> create_shader(const ShaderData& data);

		RendererType renderer_type();
	}
}
