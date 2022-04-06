#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lev;

Material::Material()
	: shader(nullptr)
{
	mem::set(textures, 0, sizeof(Ref<Texture>) * LEV_MAT_TEXTURES);
	mem::set(samplers, 0, sizeof(TextureSampler) * LEV_MAT_TEXTURES);
}

Material::Material(
	const Ref<Shader>& shd,
	const Ref<Texture>* tex,
	const TextureSampler* sampl,
	int tex_count
)
	: shader(shd)
{
	mem::copy(this->textures, tex, sizeof(Ref<Texture>) * tex_count);
	mem::copy(this->samplers, sampl, sizeof(TextureSampler) * tex_count);
}
