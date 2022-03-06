#include <leviathan.h>
#include <lev/ui/components/texture_component.h>

#include <iostream>

using namespace lev;

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

Ref<gfx::Texture> tex0;
Ref<gfx::Texture> tex1;
Ref<gfx::Texture> tex2;
Ref<gfx::Shader> shd0;

gfx::SpriteBatch batch;
ui::UIComponent ui_container;

constexpr float ROT_INTERVAL = Calc::TAU * 1/4; // 1 quarter circle rotation
float rotation_time = 0.0f;
float target_rotation = 0.0f;
float rotation = 0.0f;
float rotation_duration = 200;

void init()
{
	// load texture
	tex0 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");
	tex1 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p1.png");
	tex2 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");

	// load shader
	shd0 = gfx::Shader::create(
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\vertex.vert",
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\fragment.frag"
	);

	// assign the shaders uniform variables
	shd0->assign_uniform("u_projection", gfx::UniformType::MAT4X4, gfx::UniformFlags::PROJECTION);
	shd0->assign_uniform("u_texture", gfx::UniformType::SAMPLER2D, gfx::UniformFlags::MAIN_TEXTURE);

	// set the default shader
	batch.push_shader(shd0);

	// test out physics
	phys::Collider col0 = phys::Collider(-2, -1, 4, 2);
	phys::Collider col1 = phys::Collider(1, -1, 2, 2);
	Vec2 pushout;
	col1.overlaps(col0, &pushout);
	std::cout << pushout << std::endl;

	// ui
	ui_container.x(0);
	ui_container.y(0);
	ui_container.width(App::draw_width());
	ui_container.height(App::draw_height());

	gfx::TextureRegion uitexture = {
		.texture = tex0,
		.source = RectI(0, 0, 256, 256)
	};

	ui_container.add<ui::UITextureComponent>(ui::UIConstraints::create_fixed(0, 0, 256, 256), uitexture);
}

void update()
{
	rotation_time++;

	if (rotation_time < rotation_duration)
	{
		rotation = target_rotation + (ROT_INTERVAL * Ease::back_in_out(rotation_time / rotation_duration));
	}
	else
	{
		target_rotation += ROT_INTERVAL;
		rotation_time = 0;
	}

	ui_container.update();
}

void render()
{
	auto transform = Mat3x2::create_transform(
		Vec2(Calc::cos(Time::ticks / 100.0f) * 100.0f, Calc::sin(Time::ticks / 100.0f) * 100.0f),
		rotation,
		Vec2::ONE * (((Calc::cos(Time::ticks/50.0f)+1)*0.5f)*0.5f + 0.5f),
		Vec2(128, 128)
	);// * Mat3x2::create_skew(Vec2(Calc::sin(Time::ticks / 100.0f), Calc::cos(Time::ticks / 100.0f)));

	batch.push_matrix(transform);
	{
		batch.render_texture(tex2);

		batch.push_matrix(Mat3x2::create_transform(Vec2(512, 256), 0, Vec2::ONE, Vec2::ZERO));
		{
			batch.render_texture(tex1);
		}
		batch.pop_matrix();
	}
	batch.pop_matrix();

	ui_container.render(batch);

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
