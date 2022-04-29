#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/math/vec2.h>
#include <lev/containers/string.h>

// i dont know what would be the best size for these so this is just a guess
#define LEV_FONT_ATLAS_W 1024
#define LEV_FONT_ATLAS_H 512
#define LEV_FONT_CHARCOUNT 256

namespace lev
{
    class Texture;
	struct TextureRegion;

	enum FontType
	{
		FONT_TYPE_NONE = 0,
		FONT_TYPE_TTF,
		FONT_TYPE_OTF,
		FONT_TYPE_MAX
	};

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
			u8 type;
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
			Texture* texture;
			TextureRegion region(const RectI& rect) const;
		};

		Font();
		Font(float size, const String& path);
		~Font();

		void load(float size, const String& path);
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
