#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

#define LEV_MAT_TEXTURES 30

namespace lev
{
	class Material
	{
	public:
		Material();
		Material(
			const Ref<Shader>& shd,
			const Ref<Texture>* tex,
			const TextureSampler* sampl,
			int tex_count
		);

		Ref<Shader> shader;
		Ref<Texture> textures[LEV_MAT_TEXTURES];
		TextureSampler samplers[LEV_MAT_TEXTURES];
	};
}
