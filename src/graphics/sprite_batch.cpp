#include <lev/graphics/sprite_batch.h>

using namespace Lev;

SpriteBatch::SpriteBatch()
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::render(int x, int y, const Ref<Texture>& texture)
{
	texture->bind();

	// draw the quad
}

void SpriteBatch::set_shader(const Ref<Shader>& shader)
{
}
