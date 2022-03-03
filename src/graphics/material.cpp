#include <lev/graphics/material.h>

using namespace Lev;

Material::Material()
{
}

Material::~Material()
{
}

void Material::shader(const Ref<Shader>& shader)
{
	m_shader = shader;
}

Ref<Shader> Material::shader() const
{
	return m_shader;
}

void Material::texture(const Ref<Texture>& texture)
{
	m_texture = texture;
}

Ref<Texture> Material::texture() const
{
	return m_texture;
}

void Material::sampler(const TextureSampler& sampler)
{
	m_sampler = sampler;
}

TextureSampler Material::sampler() const
{
	return m_sampler;
}
