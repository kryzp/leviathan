#include <lev/graphics/material.h>

using namespace lev;
using namespace lev::gfx;

Material::Material()
	: textures()
	, samplers()
	, shader(nullptr)
{
}

Material::Material(const Ref<Shader>& shader, const Vector<Ref<Texture>>& textures, const Vector<TextureSampler>& samplers)
	: textures(textures)
	, samplers(samplers)
	, shader(shader)
{
}
