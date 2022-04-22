#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lev;

Material::Material()
	: m_shader(nullptr)
{
	mem::set(m_textures, 0, sizeof(Ref<Texture>) * LEV_MAT_TEXTURES);
	mem::set(m_samplers, 0, sizeof(TextureSampler) * LEV_MAT_TEXTURES);
}

Material::Material(
	const Ref<Shader>& shader,
	const Ref<Texture>* textures,
	const TextureSampler* samplers,
	u32 texture_count
)
	: m_shader(shader)
{
	mem::copy(m_textures, textures, sizeof(Ref<Texture>) * texture_count);
	mem::copy(m_samplers, samplers, sizeof(TextureSampler) * texture_count);
}

Ref<Shader>& Material::shader()
{
	return m_shader;
}

const Ref<Shader>& Material::shader() const
{
	return m_shader;
}

Ref<Texture>& Material::texture(u32 idx)
{
	return m_textures[idx];
}

const Ref<Texture>& Material::texture(u32 idx) const
{
	return m_textures[idx];
}

TextureSampler& Material::sampler(u32 idx)
{
	return m_samplers[idx];
}

const TextureSampler& Material::sampler(u32 idx) const
{
	return m_samplers[idx];
}
