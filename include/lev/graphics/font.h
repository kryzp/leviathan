#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/math/vec2.h>

// todo: make this determined by the font not constant
#define LEV_FONT_CHARCOUNT 256
#define LEV_FONT_ATLAS_W 1024
#define LEV_FONT_ATLAS_H 1024

namespace lev
{
    class Texture;
	struct TextureRegion;

	struct FontInfo
	{
		float size;
		int ascent;
		int descent;
		int line_gap;
		RectI bbox;
	};

	struct FontKerning
	{
		int advance;
		int glyph0;
		int glyph1;
	};

	struct FontCharacter
	{
		int codepoint;
		RectI bbox;
		float advance_x;
		Vec2F draw_offset;
		Vec2F draw_offset2;
	};

	struct FontAtlas
	{
		Ref<Texture> texture;
		TextureRegion region(const RectI& rect) const;
	};

	// ONLY SUPPORTS TRUETYPE FONTS
	// todo: use some kind of enum class to denote which how to load the font

	class Font
	{
	public:
		Font();
		Font(float size, const char* path);
		~Font();

		void load(float size, const char* path);
		void free();

        int kern_advance(int curr, int next) const;

		const FontInfo& info() const;
		const FontAtlas& atlas() const;
		const FontCharacter& character(int idx) const;

	private:
		FontInfo m_info;
		void* m_internal_info;

		FontKerning* m_kerning;
		int m_kerning_count;

		FontCharacter* m_characters;
		FontAtlas m_atlas;
	};
}
