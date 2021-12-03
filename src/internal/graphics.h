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
		OpenGL
	};

	namespace Graphics
	{
		bool init();
		void shutdown();

		void render(/*todo: take an argument containing what to render here? a render pass struct idk. to be used by the batch when i get to it lol :/*/);
		
		void before_render();
		void after_render();

		Ref<Texture> create_texture();
		Ref<Shader> create_shader(const ShaderData& data);

		RendererType renderer_type();
	}
}
