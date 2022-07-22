#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lev;

Ref<Material> Material::create(const Ref<Shader>& shader)
{
	auto result = create_ref<Material>();
	result->m_shader = shader;
	result->m_textures[0] = nullptr;
	result->m_samplers[0] = TextureSampler::pixel(); // idk just default setting
	return result;
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
