#include <leviathan.h>
#include <iostream>

using namespace Lev;

namespace
{
	constexpr int WINDOW_WIDTH = 1280;
	constexpr int WINDOW_HEIGHT = 720;

	Ref<Texture> tex0;
	Ref<Shader> shd0;
	SpriteBatch batch;

	float time = 0.0f;

	float rotation_time = 0.0f;
	float target_rotation = 0.0f;
	float rotation = 0.0f;
	float rotation_duration = 200.0f;

	void init()
	{
		tex0 = Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");

		shd0 = Shader::create(
			"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\vertex.vert",
			"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\fragment.frag"
		);
		
		shd0->assign_uniform("u_projection", UniformType::Mat4x4, UniformFlags::Projection);
		shd0->assign_uniform("u_texture", UniformType::Sampler2D, UniformFlags::MainTexture);

		// set the default shader
		batch.push_shader(shd0);
	}

	void update()
	{
		time++; 
		rotation_time++;

		float t = rotation_time / rotation_duration;
		if (rotation_time < rotation_duration)
		{
			t = Ease::exp_in_out(t);
			rotation = target_rotation + Calc::PI*t;
		}
		else
		{
			target_rotation += Calc::PI;
			rotation_time = 0;
		}
	}

	void render()
	{
		Mat3x2 transformation = Mat3x2::create_transformation(
			Vec2(0, Calc::sin(time / 100.0f) * 100.0f),
			rotation,
			Vec2::ONE * (Calc::cos(time / 100.0f)+1) * 0.5f,
			Vec2(128, 128)
		);

		batch.push_matrix(transformation);
		batch.render_texture(tex0);
		batch.pop_matrix();
		batch.render(Mat4x4::create_orthographic(1280.0f, 720.0f, 0.0f, 1000.0f));
	}
}

int main()
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "leviathan demo";
	config.width = WINDOW_WIDTH;
	config.height = WINDOW_HEIGHT;
	config.target_framerate = 60;
	config.resizable = true;
	config.on_update = update;
	config.on_render = render;
	config.on_init = init;

	App::start(&config);

	return 0;
}
