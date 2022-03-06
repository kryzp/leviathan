#include <lev/graphics/material.h>

using namespace lev;
using namespace lev::gfx;

Material::Material()
{
}

Material::~Material()
{
}

Ref<Shader>& Material::shader()
{
	return m_shader;
}

const Ref<Shader>& Material::shader() const
{
	return m_shader;
}

Ref<Texture>& Material::texture()
{
	return m_texture;
}

const Ref<Texture>& Material::texture() const
{
	return m_texture;
}

TextureSampler& Material::sampler()
{
	return m_sampler;
}
