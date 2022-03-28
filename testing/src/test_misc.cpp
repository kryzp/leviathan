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

void init()
{
	g_font = create_ref<Font>(60, "D:\\_PROJECTS\\leviathan\\testing\\res\\fonts\\arial.ttf");
}

void render()
{
	g_text += Input::text();

	if (Input::pressed(Key::BACKSPACE) && g_text.length() > 0)
		g_text.pop_back();

	App::clear(0xFFBB23FF);
	{
		g_batch.push_matrix(Mat3x2::create_translation(Input::mouse_position()));
		g_batch.push_string(g_text.c_str(), g_font, [&](FontCharacter c, int idx) { return Vec2F(Calc::sin(Time::seconds * 2 + idx*100) * 100.0f, Calc::sin(Time::seconds + idx*50) * 50.0f); });
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

/*
#include "../../ext/stupid/int128.h"

using namespace lev;

int main(void)
{
	stp::i128 integer0 = "";
	stp::i128 integer1 = "";
	stp::i128 integer2 = stp::i128::add(integer0, integer1);

	integer2.print();

	return 0;
}
*/
