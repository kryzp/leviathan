#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lev;

Material::Material()
	: m_shader(nullptr)
{
	mem::set(m_textures, 0, sizeof(Texture*) * LEV_MAT_TEXTURES);
	mem::set(m_samplers, 0, sizeof(TextureSampler) * LEV_MAT_TEXTURES);
}

Material::Material(
	Shader* shader,
	Texture** textures,
	TextureSampler* samplers,
	u32 texture_count
)
	: m_shader(shader)
{
	mem::copy(m_textures, textures, sizeof(Texture*) * texture_count);
	mem::copy(m_samplers, samplers, sizeof(TextureSampler) * texture_count);
}

Shader* Material::shader()
{
	return m_shader;
}

const Shader* Material::shader() const
{
	return m_shader;
}

void Material::set_shader(Shader* shader)
{
	m_shader = shader;
}

Texture* Material::texture(u32 idx)
{
	return m_textures[idx];
}

const Texture* Material::texture(u32 idx) const
{
	return m_textures[idx];
}

void Material::set_texture(u32 idx, Texture* texture)
{
	m_textures[idx] = texture;
}

TextureSampler& Material::sampler(u32 idx)
{
	return m_samplers[idx];
}

const TextureSampler& Material::sampler(u32 idx) const
{
	return m_samplers[idx];
}

void Material::set_sampler(u32 idx, TextureSampler sampler)
{
	m_samplers[idx] = sampler;
}
