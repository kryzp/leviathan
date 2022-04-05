#include <lev/core/app.h>

#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/node/node.h>

#include <lev/node/2d/collider_2d.h>
#include <lev/node/2d/node_2d.h>
#include <lev/node/2d/mover_2d.h>
#include <lev/node/2d/sprite_2d.h>

using namespace lev;

int main()
{
	SpriteBatch batch;
	Node scene;
	Ref<Texture> tex;
	TextureRegion texrgn(nullptr, RectI(256, 0, 256, 256));

	Config conf;
	{
		conf.name = "ah yes: n o d e";
		//conf.vsync = false;

		conf.on_init = [&]()
		{
			tex = AssetMgr::inst()->load<Texture>("tex", "D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");
			texrgn.source = tex;
			
			auto mover = scene.add<Mover2D>();
			{
				mover->set_velocity(Vec2F(16.0f, 8.0f));

				auto collider = mover->add<Collider2D>();
				{
					collider->make_rect(RectF(0, 0, 256, 256));
					mover->set_collider(collider);
				}

				auto sprite = mover->add<Sprite2D>(texrgn);
			}
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
