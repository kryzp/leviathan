#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

#define LEV_MAT_TEXTURES 32

namespace lev
{
	class Material
	{
	public:
		Material();
		Material(
			const Ref<Shader>& shaders,
			const Ref<Texture>* textures,
			const TextureSampler* samplers,
            u32 texture_count
		);

		Ref<Shader>& shader();
		const Ref<Shader>& shader() const;

		Ref<Texture>& texture(u32 idx);
		const Ref<Texture>& texture(u32 idx) const;

		TextureSampler& sampler(u32 idx);
		const TextureSampler& sampler(u32 idx) const;

	private:
		Ref<Shader> m_shader;
		Ref<Texture> m_textures[LEV_MAT_TEXTURES];
		TextureSampler m_samplers[LEV_MAT_TEXTURES];
	};
}
