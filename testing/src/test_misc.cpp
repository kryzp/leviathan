#include <leviathan.h>
#include <lev/ui/components/texture_component.h>

int main(void)
{
	lev::SpriteBatch batch;
	lev::Ref<lev::Texture> tex;
	lev::UIComponent uimain;

	lev::AppConfig conf;
	{
		conf.name = "yes";
		conf.width = 1280;
		conf.height = 720;
		conf.target_framerate = 60;
		conf.resizable = true;
		conf.vsync = true;

		conf.on_init = [&]() -> void {
			tex = lev::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");

			uimain.add<lev::UITextureComponent>(
				lev::UIConstraints::create_fixed(10, 10, 256, 256),
				lev::TextureRegion(tex, lev::RectF(0.0f, 0.0f, 256.0f, 256.0f))
			);
		};

		conf.on_update = [&]() -> void {
			uimain.update();
		};
		
		conf.on_render = [&]() -> void {
			uimain.render(batch);
			batch.render();
		};
	}
	lev::App::start(conf);

	return 0;
}
