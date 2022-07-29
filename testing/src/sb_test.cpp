#include <lev/graphics/sprite_batch.h>
#include <lev/core/app.h>
#include <lev/input/input.h>

using namespace lev;

int main(int argc, char** argv)
{
	lev::Config conf;
	{
		conf.name = "sb_test.exe";
		conf.width = 1280;
		conf.height = 720;

		SpriteBatch b;

		conf.on_render = [&]() -> void
		{
			App::clear();
			{
				static float xxx = 300.0f;
				xxx += 5.5f;

				b.push_triangle_outline(Triangle(
					Vec2F(200.0f, 200.0f),
					Vec2F(100.0f, 300.0f),
					Input::inst()->mouse_position()
				), 10.0f, Colour::white());

				b.push_circle_outline(Circle(
					Vec2F(50.0f, 50.0f),
					10.0f
				), 1.0f, Colour::white(), 40U);
			}
			b.render();
		};
	}
	lev::App::inst()->start(conf);

	return 0;
}
