#include <lev/assets/asset_mgr.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/font.h>
#include <lev/io/file_stream.h>

using namespace lev;

class TextureLoader : public AssetLoader<Texture>
{
public:
	Ref<Texture> load(const String& path) override;
};

class ShaderLoader : public AssetLoader<Shader>
{
public:
	Ref<Shader> load(const String& path) override;
};

class FontLoader : public AssetLoader<Font>
{
public:
	Ref<Font> load(const String& path) override;
};

Ref<Texture> TextureLoader::load(const String& path)
{
	LEV_ASSERT(!obj, "Object being initialized must not currently exist");
	return Texture::create(path.c_str());
}

Ref<Shader> ShaderLoader::load(const String& path)
{
	LEV_ASSERT(!obj, "Object being initialized must not currently exist");

	FileStream file(path.c_str(), "r");
	String line = "";
	s32 getline_cache = 0;

	float size = 0.0f;
	Ref<Shader> result = nullptr;

	while (file.get_line(line, getline_cache))
	{
		if (line.starts_with("# opengl"))
		{
			Vector<String> gl_field_content;

			String opengl_line = "";
			s32 getline_cache_gl = getline_cache;

			String vert, frag, geom, comp;

			while (file.get_line(opengl_line, getline_cache_gl))
			{
				if (opengl_line.starts_with("# end"))
					break;

				gl_field_content.push_back(opengl_line);
			}

			for (auto& content : gl_field_content)
			{
				auto trimmed = content.trim();

				Vector<String> tokens;
				tokens = content.split(":");

				for (auto& str : tokens) { str = str.trim(); }

				if 		(trimmed.starts_with("vert")) vert = tokens[1];
				else if (trimmed.starts_with("frag")) frag = tokens[1];
				else if (trimmed.starts_with("geom")) geom = tokens[1];
				else if (trimmed.starts_with("comp")) comp = tokens[1];
			}

			if (!comp.empty())
				result = Shader::create_compute_seperated(comp);
			else
				result = Shader::create_seperated(vert, frag, geom);

			break;
		}
	}

	file.close();

	return result;
}

Ref<Font> FontLoader::load(const String& path)
{
	LEV_ASSERT(!obj, "Object being initialized must not currently exist");

	FileStream file(path.c_str(), "r");
	String line;

	float size;
	String font_path;

	s32 getline_offset = 0;

	while (file.get_line(line, getline_offset))
	{
		auto trimmed = line.trim();

		auto tokens = line.split(":");
		for (auto& str : tokens) { str = str.trim(); }

		if (trimmed.starts_with("path"))
		{
			font_path = tokens[1];
		}
		else if (trimmed.starts_with("size"))
		{
			size = cstr::to_float(tokens[1].c_str());
		}
	}

	return create_ref<Font>(size, font_path.c_str());
}

AssetMgr::AssetMgr()
	: m_counter(0)
	, m_loaders()
	, m_assets()
{
	register_loader<TextureLoader, Texture>();
	register_loader<ShaderLoader, Shader>();
	register_loader<FontLoader, Font>();
}

AssetMgr::~AssetMgr()
{
	free_load_data<Texture>();
	free_load_data<Shader>();
	free_load_data<Font>();
}
