#include <lev/graphics/font.h>
#include <lev/math/calc.h>
#include <lev/io/file_stream.h>
#include <lev/graphics/texture.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <third_party/stb/stb_truetype.h>

using namespace lev;

#define LEV_FONT_ATLAS_SIZE (LEV_FONT_ATLAS_W*LEV_FONT_ATLAS_H)
#define M_INTERNAL_INFO ((stbtt_fontinfo*)m_internal_info)

TextureRegion FontAtlas::region(const RectI& rect) const
{
	return TextureRegion(texture, rect);
}

///////////////////////////////////////////////////

Font::Font()
	: m_info()
	, m_internal_info(nullptr)
	, m_kerning(nullptr)
	, m_kerning_count(0)
	, m_characters(nullptr)
	, m_atlas()
{
}

Font::Font(float size, const char* path)
	: Font()
{
	load(size, path);
}

Font::~Font()
{
	free();
}

void Font::load(float size, const char* path)
{
	LEV_ASSERT(path, "Path must not be null");
	LEV_ASSERT(size > 0.0f, "Size must be greater than 0");

	FileStream fs(path, "rb");
	byte* ttf_buffer = new byte[fs.size()];
	fs.read(ttf_buffer, fs.size()).close();

	m_internal_info = new stbtt_fontinfo();
	if (!stbtt_InitFont(M_INTERNAL_INFO, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0)))
		LEV_ERROR("Failed to create font");

	m_info.size = size;

	stbtt_GetFontVMetrics(M_INTERNAL_INFO, &m_info.ascent, &m_info.descent, &m_info.line_gap);

	int x0, y0, x1, y1;
	stbtt_GetFontBoundingBox(M_INTERNAL_INFO, &x0, &y0, &x1, &y1);
	{
		m_info.bbox.x = x0;
		m_info.bbox.y = y0;
		m_info.bbox.w = x1 - x0;
		m_info.bbox.h = y1 - y0;
	}

	// store kerning data
	m_kerning_count = stbtt_GetKerningTableLength(M_INTERNAL_INFO);
	m_kerning = new FontKerning[m_kerning_count];
	{
		stbtt_kerningentry* kerning_tables = new stbtt_kerningentry[m_kerning_count];
		stbtt_GetKerningTable(M_INTERNAL_INFO, kerning_tables, m_kerning_count);

		for (int i = 0; i < m_kerning_count; i++)
		{
			m_kerning[i] = {
				.advance = kerning_tables[i].advance,
				.glyph0 = kerning_tables[i].glyph1,
				.glyph1 = kerning_tables[i].glyph2
			};
		}

		delete[] kerning_tables;
	}
	
	// pack data into texture + store character data
	byte* bitmap = new byte[LEV_FONT_ATLAS_SIZE];
	{
		stbtt_packedchar packed_chars[LEV_FONT_CHARCOUNT];
		stbtt_pack_context pack_context = { 0 };

		if (!stbtt_PackBegin(&pack_context, bitmap, LEV_FONT_ATLAS_W, LEV_FONT_ATLAS_H, LEV_FONT_ATLAS_W, 1, NULL))
			LEV_ERROR("Failed to initialize font");

		//stbtt_PackSetOversampling(&pack_context, m_info.oversample_x, m_info.oversample_y);

		if (!stbtt_PackFontRange(&pack_context, ttf_buffer, 0, m_info.size, 0, LEV_FONT_CHARCOUNT, packed_chars))
			LEV_ERROR("Failed to pack font");

		stbtt_PackEnd(&pack_context);

		m_atlas.texture = Texture::create(LEV_FONT_ATLAS_W, LEV_FONT_ATLAS_H, TextureFormat::RED, bitmap);

		m_characters = new FontCharacter[LEV_FONT_CHARCOUNT];

		for (int i = 0; i < LEV_FONT_CHARCOUNT; i++)
		{
			const auto& c = packed_chars[i];
			
			m_characters[i] = {
				.codepoint = i,
				.bbox = RectI(c.x0, c.y0, c.x1 - c.x0, c.y1 - c.y0),
				.advance_x = c.xadvance,
				.draw_offset = Vec2F(c.xoff, c.yoff),
				.draw_offset2 = Vec2F(c.xoff2, c.yoff2)
			};
		}
	}
	delete[] bitmap;

	delete[] ttf_buffer;
}

void Font::free()
{
	if (m_internal_info)
		delete M_INTERNAL_INFO;

	if (m_kerning)
		delete[] m_kerning;

	if (m_characters)
		delete[] m_characters;
}

int Font::kern_advance(int curr, int next) const
{
	for (int i = 0; i < m_kerning_count; i++)
	{
		auto kern = m_kerning[i];
		
		if (kern.glyph0 == curr && kern.glyph1 == next)
			return kern.advance;
	}

	return 0;
}

const FontInfo& Font::info() const
{
	return m_info;
}

const FontAtlas& Font::atlas() const
{
	return m_atlas;
}

const FontCharacter& Font::character(int idx) const
{
	LEV_ASSERT(idx >= 0 && idx < LEV_FONT_CHARCOUNT, "Index must be within bounds of the character array [0 -> 255]");
	return m_characters[idx];
}
