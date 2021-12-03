#include <leviathan.h>
#include <iostream>

using namespace Lev;

namespace
{
	Vector<int> random_integers = { 5, 3, 6, 8, 2, 4, 7 };

	void startup()
	{
		int ii = random_integers.pop_front();

		std::cout << "E:" << ii << std::endl;

        for(int i : random_integers)
            std::cout << i << std::endl;
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
