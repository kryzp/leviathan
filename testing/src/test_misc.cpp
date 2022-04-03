#include <lev/core/app.h>
#include <lev/math/rng.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/assets/asset_mgr.h>

using namespace lev;

int main()
{
	SpriteBatch batch;
	Rng rng;

	Config conf;
	{
		conf.name = "ah yes: p a r t i c l e";
		//conf.vsync = false;

		conf.on_init = [&]()
		{
		};

		conf.on_update = [&]()
		{
		};

		conf.on_render = [&]()
		{
			App::clear(Colour::blue());
			batch.render();
		};
	}
	App::start(conf);

	return 0;
}
