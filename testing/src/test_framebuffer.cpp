#include <leviathan.h>

using namespace lev;

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

void init()
{
}

void destroy()
{
}

void update()
{
}

void render()
{
}

int main(void)
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "framebuffer demo";
	config.width = WINDOW_WIDTH;
	config.height = WINDOW_HEIGHT;
	config.target_framerate = 60;
	config.resizable = true;
	config.on_init = init;
	config.on_destroy = destroy;
	config.on_update = update;
	config.on_render = render;

	App::start(&config);

	return 0;
}
