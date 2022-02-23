#include <leviathan.h>
#include <iostream>

using namespace Lev;

namespace
{
<<<<<<< HEAD
	// i have been putting off doing the graphics backend for so long now :/
	// time to start learning some opengl stuff i guess...

	Ref<Texture> tex0;
	Ref<Shader> shd0;
	SpriteBatch batch;

	void init()
	{
		tex0 = Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\txrs\\p0.png");

		shd0 = Shader::create(
			"D:\\_PROJECTS\\leviathan\\testing\\res\\shdrs\\vertex.vert",
			"D:\\_PROJECTS\\leviathan\\testing\\res\\shdrs\\fragment.frag"
		);

		batch.set_shader(shd0);
=======
	void startup()
	{
>>>>>>> e5268c6ecd106d46bc506f7496d77d5c1f83ddfb
	}

	void update()
	{
		static int time = 0;
		shd0->set("u_time", time);
		time++;
	}

	void render()
	{
		batch.render(0, 0, tex0);
	}
}

int main()
{
	AppConfig config;
	config.name = "leviathan demo";
	config.width = 1280;
	config.height = 720;
	config.target_framerate = 60;
	config.on_update = update;
	config.on_render = render;
	config.on_init = init;

	App::start(&config);

	return 0;
}
