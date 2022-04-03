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

	// ONLY SUPPORTS TRUETYPE FONTS
	// todo: use some kind of enum class to denote which how to load the font

	class Font
	{
	public:
		struct Info
		{
			float size;
			int ascent;
			int descent;
			int line_gap;
			RectI bbox;
		};

		struct Kerning
		{
			int advance;
			int glyph0;
			int glyph1;
		};

		struct Character
		{
			int codepoint;
			RectI bbox;
			float advance_x;
			Vec2F draw_offset;
			Vec2F draw_offset2;
		};

		struct Atlas
		{
			Ref<Texture> texture;
			TextureRegion region(const RectI& rect) const;
		};

		Font();
		Font(float size, const char* path);
		~Font();

		void load(float size, const char* path);
		void free();

		float string_width(const char* str) const;
		float string_height(const char* str) const;

        int kern_advance(int curr, int next) const;

		const Info& info() const;
		const Atlas& atlas() const;
		const Character& character(int idx) const;

	private:
		Info m_info;
		void* m_internal_info;

		Kerning* m_kerning;
		int m_kerning_count;

		Character* m_characters;
		Atlas m_atlas;
	};
}
