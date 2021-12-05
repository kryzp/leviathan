#include <leviathan.h>
#include <iostream>

using namespace Lev;

namespace
{
	void startup()
	{
	}

	void update()
	{
	}

	void render()
	{
	}
}

int main()
{
	Config config;
	config.name = "leviathan demo";
	config.width = 1280;
	config.height = 720;
	config.target_framerate = 60;
	config.on_update = update;
	config.on_render = render;
	config.on_startup = startup;

	App::start(&config);

	return 0;
}
