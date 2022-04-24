#include <lev/assets/asset_mgr.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/font.h>

using namespace lev;

class TextureLoader : public AssetLoader<Texture, const String&>
{
public:
	void load(Ref<Texture>& obj, const String& path) override
	{
		obj = Texture::create(path);
	}
};

class ShaderLoader : public AssetLoader<Shader, ShaderLoadData>
{
public:
	void load(Ref<Shader>& obj, ShaderLoadData data) override
	{
		if (data.compute)
			obj = Shader::create_compute(data.compute, data.is_source);
		else
			obj = Shader::create(data.vertex, data.fragment, data.geometry, data.is_source);
	}
};

class FontLoader : public AssetLoader<Font, FontLoadData>
{
public:
	void load(Ref<Font>& obj, FontLoadData data) override
	{
		obj = create_ref<Font>(data.size, data.path);
	}
};

AssetMgr::AssetMgr()
	: m_registry()
{
	mem::set(m_loaders, 0, sizeof(AssetLoaderBase*) * LEV_MAX_ASSET_TYPES);
	mem::set(m_assets, 0, sizeof(AssetListBase*) * LEV_MAX_ASSET_TYPES);

	register_loader<TextureLoader, Texture>();
	register_loader<ShaderLoader, Shader>();
	register_loader<FontLoader, Font>();
}
