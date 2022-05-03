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
			Shader* shaders,
			Texture** textures,
			TextureSampler* samplers,
            u32 texture_count
		);

		Shader* shader();
		const Shader* shader() const;
		void set_shader(Shader* shader);

		Texture* texture(u32 idx);
		const Texture* texture(u32 idx) const;
		void set_texture(u32 idx, Texture* texture);

		TextureSampler& sampler(u32 idx);
		const TextureSampler& sampler(u32 idx) const;
		void set_sampler(u32 idx, TextureSampler sampler);

	private:
		Shader* m_shader;

		// todo Vector<T>???
		Texture* m_textures[LEV_MAT_TEXTURES];
		TextureSampler m_samplers[LEV_MAT_TEXTURES];
	};
}
