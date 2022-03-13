#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

namespace lev::gfx
{
	class Material
	{
	public:
		Material();
		Material(const Ref<Shader>& shader, const Vector<Ref<Texture>>& textures, const Vector<TextureSampler>& samplers);

		Vector<Ref<Texture>> textures;
		Vector<TextureSampler> samplers;
		Ref<Shader> shader;
	};
}
