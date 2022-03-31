#include <lev/core/app.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/node/gui/gui_constraints.h>
#include <lev/node/gui/components/clickable_texture.h>

int main(void)
{
	lev::SpriteBatch batch;
	lev::Node scene;
	lev::Ref<lev::Texture> texture;

	lev::Config conf;
	{
		conf.name = "yes";
		conf.width = 1280;
		conf.height = 720;
		conf.target_framerate = 60;
		conf.resizable = true;
		conf.vsync = true;

		conf.on_init = [&]() -> void
		{
			texture = lev::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");
			auto texrgn = lev::TextureRegion(texture, lev::RectI(0, 0, 256, 256));

			scene.add<lev::GUIClickableTexture>(
				lev::GUIConstraints::create_fixed(10, 10, 100, 100),
				texrgn,
				[&](lev::GUIClickableTexture::ClickData data) -> void
				{
					lev::Log::print("Clicked at (%d, %d) relative to me :)", data.where.x, data.where.y);
				}
			);
		};

		conf.on_update = [&]() -> void
		{
			scene.update();
		};
		
		conf.on_render = [&]() -> void
		{
			scene.render(batch);
			batch.render();
		};
	}
	lev::App::start(conf);

	return 0;
}
