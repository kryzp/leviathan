#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

#define LEV_MAT_TEXTURES 32

namespace lv
{
	class Material
	{
	public:
		Material() = default;
		~Material() = default;

		static Ref<Material> create(const Ref<Shader>& shader);

		Ref<Shader> shader();
		const Ref<Shader> shader() const;
		void set_shader(const Ref<Shader>& shader);

		Ref<Texture> texture(unsigned idx);
		const Ref<Texture> texture(unsigned idx) const;
		void set_texture(const Ref<Texture>& texture, unsigned idx);

		TextureSampler& sampler(unsigned idx);
		const TextureSampler& sampler(unsigned idx) const;
		void set_sampler(const TextureSampler& sampler, unsigned idx);

	private:
		Ref<Shader> m_shader;

		// todo Vector<T>???
		Ref<Texture> m_textures[LEV_MAT_TEXTURES];
		TextureSampler m_samplers[LEV_MAT_TEXTURES];
	};
}
