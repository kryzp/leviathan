#include <leviathan.h>
#include <iostream>

using namespace lev;

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

Ref<gfx::Texture> tex0;
Ref<gfx::Texture> tex1;

gfx::SpriteBatch batch;

constexpr float ROT_INTERVAL = Calc::TAU * 1/4;
float rotation_time = 0.0f;
float target_rotation = 0.0f;
float rotation = 0.0f;
float rotation_duration = 200;

void init()
{
	batch.initialize();

	tex0 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p1.png");
	tex1 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");
}

void update()
{
	rotation_time++;

	if (rotation_time < rotation_duration)
	{
		rotation = target_rotation + (ROT_INTERVAL * Ease::elastic_in_out(rotation_time / rotation_duration));
	}
	else
	{
		target_rotation += ROT_INTERVAL;
		rotation_time = 0;
	}
}

void render()
{
	App::clear(Colour(0, 190, 170));

	auto transform = Mat3x2::create_transform(
		Vec2(Calc::cos(Time::ticks / 100.0f) * 100.0f, Calc::sin(Time::ticks / 100.0f) * 100.0f),
		rotation,
		Vec2::ONE * (((Calc::cos(Time::ticks/50.0f)+1)*0.5f)*0.5f + 0.5f),
		Vec2(128, 128)
	);

	batch.push_matrix(transform);
	{
		batch.render_texture(tex1);

		batch.push_matrix(Mat3x2::create_transform(Vec2(512, 256), 0, Vec2::ONE, Vec2::ZERO));
		{
			batch.render_texture(tex0);
		}
		batch.pop_matrix();
	}
	batch.pop_matrix();

	batch.render(Mat4x4::create_orthographic(1280.0f, 720.0f, 0.0f, 1000.0f));
}

int main(void)
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "le spinne demo";
	config.width = WINDOW_WIDTH;
	config.height = WINDOW_HEIGHT;
	config.target_framerate = 60;
	config.resizable = true;
	config.on_init = init;
	config.on_update = update;
	config.on_render = render;

	App::start(&config);

	return 0;
}
