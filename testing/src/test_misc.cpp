#include <lev/core/app.h>

#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/node/node.h>
#include <lev/math/range.h>

#include <lev/node/2d/collider_2d.h>
#include <lev/node/2d/node_2d.h>
#include <lev/node/2d/mover_2d.h>
#include <lev/node/2d/sprite_2d.h>

using namespace lev;

int main()
{
	SpriteBatch batch;
	Node scene;

	Config conf;
	{
		conf.name = "brrrrrrrrrr";
		//conf.vsync = false;

		conf.on_init = [&]()
		{
		};

		conf.on_update = [&]()
		{
			scene.update();
		};

		conf.on_render = [&]()
		{
			App::clear();
			scene.render(batch);
			batch.render();
		};
	}
	App::start(conf);

	return 0;
}
