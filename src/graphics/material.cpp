#include <lev/graphics/material.h>
#include <lev/core/util.h>

using namespace lev;

Material::Material()
	: shader(nullptr)
{
	MemUtil::clear(textures, sizeof(Ref<Texture>) * LEV_MAT_TEXTURES);
	MemUtil::clear(samplers, sizeof(TextureSampler) * LEV_MAT_TEXTURES);
}

Material::Material(
	const Ref<Shader>& shd,
	const Ref<Texture>* tex,
	const TextureSampler* sampl,
	int tex_count
)
	: shader(shd)
{
	MemUtil::copy(this->textures, tex, sizeof(Ref<Texture>) * tex_count);
	MemUtil::copy(this->samplers, sampl, sizeof(TextureSampler) * tex_count);
}
