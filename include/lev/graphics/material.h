#pragma once

#include <lev/core/util.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

namespace lev::gfx
{
	class Material
	{
	public:
		Material();
		~Material();

		Ref<Shader>& shader();
		const Ref<Shader>& shader() const;

		Ref<Texture>& texture();
		const Ref<Texture>& texture() const;
		
		TextureSampler& sampler();

	private:
		Ref<Texture> m_texture;
		TextureSampler m_sampler;
		Ref<Shader> m_shader;
	};
}
