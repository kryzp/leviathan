#include <lev/core/app.h>
#include <lev/node/node.h>
#include <lev/node/gui/components/text.h>
#include <lev/graphics/sprite_batch.h>

using namespace lev;

int main()
{
	Ref<Shader> compute_shader;
	Ref<Texture> tex;
	SpriteBatch batch;

	Config conf;
	{
		conf.on_init = [&]()
		{
			tex = Texture::create(512, 512, lev::TEXTURE_FORMAT_RGBA, nullptr);
			compute_shader = Shader::create_compute("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\compute.comp");
		};

		conf.on_update = [&]()
		{
			Log::print("fps: %f", App::fps());
		};

		conf.on_render = [&]()
		{
			App::clear();

			tex->bind_image(0);

			compute_shader->use()
				.dispatch_compute(tex->width(), tex->height(), 1)
				.wait_compute();

			batch.push_texture(tex);

			batch.render();
		};
	}
	App::start(conf);
	return 0;
}
