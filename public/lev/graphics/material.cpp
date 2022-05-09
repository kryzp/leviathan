#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lv;

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
	unsigned texture_count
)
	: m_shader(shader)
{
	mem::copy(m_textures, textures, sizeof(Ref<Texture>) * texture_count);
	mem::copy(m_samplers, samplers, sizeof(TextureSampler) * texture_count);
}

Ref<Shader> Material::shader()
{
	return m_shader;
}

const Ref<Shader> Material::shader() const
{
	return m_shader;
}

void Material::set_shader(const Ref<Shader>& shader)
{
	m_shader = shader;
}

Ref<Texture> Material::texture(unsigned idx)
{
	return m_textures[idx];
}

const Ref<Texture> Material::texture(unsigned idx) const
{
	return m_textures[idx];
}

void Material::set_texture(const Ref<Texture>& texture, unsigned idx)
{
	m_textures[idx] = texture;
}

TextureSampler& Material::sampler(unsigned idx)
{
	return m_samplers[idx];
}

const TextureSampler& Material::sampler(unsigned idx) const
{
	return m_samplers[idx];
}

void Material::set_sampler(const TextureSampler& sampler, unsigned idx)
{
	m_samplers[idx] = sampler;
}
