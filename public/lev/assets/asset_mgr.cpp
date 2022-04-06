#include <lev/assets/asset_mgr.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/font.h>
#include <lev/containers/string.h>

using namespace lev;

class TextureLoader : public AssetLoader<Texture, const char*>
{
public:
	Ref<Texture> load(const char* name, const char* path) override
	{
		if (m_textures.contains(name))
			return m_textures.get(name);

		Ref<Texture> tex = Texture::create(path);
		m_textures.insert(name, tex);
		return tex;
	}

	Ref<Texture> get(const char* name) override
	{
		return m_textures.get(name);
	}

	bool has(const char* name) override
	{
		return m_textures.contains(name);
	}

private:
	HashMap<String, Ref<Texture>> m_textures;
};

class ShaderLoader : public AssetLoader<Shader, ShaderLoadData>
{
public:
	Ref<Shader> load(const char* name, ShaderLoadData data) override
	{
		if (m_shaders.contains(name))
			return m_shaders.get(name);

		Ref<Shader> shd = nullptr;

		if (data.compute)
			shd = Shader::create_compute(data.compute, data.is_source);
		else
			shd = Shader::create(data.vertex, data.fragment, data.geometry, data.is_source);

		m_shaders.insert(name, shd);
		return shd;
	}

	Ref<Shader> get(const char* name) override
	{
		return m_shaders.get(name);
	}

	bool has(const char* name) override
	{
		return m_shaders.contains(name);
	}

private:
	HashMap<String, Ref<Shader>> m_shaders;
};

class FontLoader : public AssetLoader<Font, FontLoadData>
{
public:
	Ref<Font> load(const char* name, FontLoadData data) override
	{
		if (m_fonts.contains(name))
			return m_fonts.get(name);

		Ref<Font> fnt = create_ref<Font>(data.size, data.path);
		m_fonts.insert(name, fnt);
		return fnt;
	}

	Ref<Font> get(const char* name) override
	{
		return m_fonts.get(name);
	}

	bool has(const char* name) override
	{
		return m_fonts.contains(name);
	}

private:
	HashMap<String, Ref<Font>> m_fonts;
};

AssetMgr::AssetMgr()
{
	register_loader<TextureLoader>();
	register_loader<ShaderLoader>();
	register_loader<FontLoader>();
}
