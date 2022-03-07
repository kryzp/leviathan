#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

namespace lev::gfx
{
	class Material
	{
	public:
		Ref<Texture> texture;
		TextureSampler sampler;
		Ref<Shader> shader;
	};
}
