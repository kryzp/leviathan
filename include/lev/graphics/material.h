#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

namespace Lev
{
	class Material
	{
	public:
		Material();
		~Material();

		void shader(const Ref<Shader>& shader);
		Ref<Shader> shader() const;

		void texture(const Ref<Texture>& texture);
		Ref<Texture> texture() const;
		
		void sampler(const TextureSampler& sampler);
		TextureSampler sampler() const;

	private:
		Ref<Texture> m_texture;
		TextureSampler m_sampler;
		Ref<Shader> m_shader;
	};
}
