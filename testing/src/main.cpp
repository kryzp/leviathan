#include <leviathan.h>
#include <iostream>

using namespace Lev;

namespace
{
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
	}

	void update()
	{
		static int time = 0;
		shd0->set("u_time", time);
		time++;

		Mat3x2 transform = Mat3x2::create_transformation(
			Vec2::ZERO,
			-time * Calc::DEG2RAD * 1/2,
			Vec2::ONE,
			Vec2::ZERO
		);

		shd0->set("u_transform", transform.to_mat4x4_transform());
	}

	void render()
	{
		shd0->set("u_texture", 0);
		tex0->bind();
		shd0->use();
		batch.render_quad(0, 0);
	}
}

int main()
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\res\\logs\\");

	AppConfig config;
	config.name = "leviathan demo";
	config.width = 1280;
	config.height = 720;
	config.target_framerate = 60;
	config.resizable = true;
	config.on_update = update;
	config.on_render = render;
	config.on_init = init;

	App::start(&config);

	return 0;
}
