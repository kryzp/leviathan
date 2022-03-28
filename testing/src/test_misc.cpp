#include <leviathan.h>
#include "../../ext/collider.h"
#include <iostream>
#include <string>

using namespace lev;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

SpriteBatch g_batch;
Ref<Font> g_font;
std::string g_text;

Colour modulate_colour()
{
	return Colour::from_hsv(Calc::mod(Time::seconds * 256.0f, 360.0f), 1.0f, 1.0f);
}

void init()
{
	g_font = create_ref<Font>(60, "D:\\_PROJECTS\\leviathan\\testing\\res\\fonts\\nokiafc22.ttf");
}

void render()
{
	g_text += Input::text();

	if (Input::pressed(Key::BACKSPACE) && g_text.length() > 0)
		g_text.pop_back();

	App::clear(0xFFBB23FF);
	{
		g_batch.push_matrix(Mat3x2::create_translation(Input::mouse_position()));

		g_batch.push_string(
			g_text.c_str(),
			g_font,
			[&](FontCharacter c, int idx) {
				return Vec2F(
					Calc::cos(Time::seconds * 2 + idx*100) * 50.0f,
					Calc::sin(Time::seconds + idx*50) * 50.0f);
			},
			modulate_colour()
		);

		g_batch.pop_matrix();
	}
	g_batch.render();
}

int main(void)
{
	AppConfig conf;
	conf.name = "yes";
	conf.width = WINDOW_WIDTH;
	conf.height = WINDOW_HEIGHT;
	conf.target_framerate = 60;
	conf.resizable = true;
	conf.vsync = true;
	conf.on_init = init;
	conf.on_render = render;

	App::start(conf);

	return 0;
}
