#include <leviathan.h>

using namespace lev;

constexpr int PICTURES = 25;
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

Ref<gfx::Framebuffer> g_framebuffer;
Ref<gfx::Texture> g_texture;
Ref<gfx::Shader> g_postprocess;
gfx::SpriteBatch g_batch;
Mat3x2 g_positions[PICTURES];
Rng g_rng;

void init()
{
	g_batch.initialize();

	g_framebuffer = gfx::Framebuffer::create(1280, 720);
	g_texture = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");
	
	g_postprocess = gfx::Shader::create(
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.vert",
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.frag"
	);

	g_postprocess->assign_uniform("u_projection", gfx::UniformType::MAT4X4, gfx::UniformFlags::PROJECTION);
	g_postprocess->assign_uniform("u_texture", gfx::UniformType::SAMPLER2D, gfx::UniformFlags::MAIN_TEXTURE);
	g_postprocess->assign_uniform("u_time", gfx::UniformType::INTEGER, gfx::UniformFlags::NONE);

	for (int i = 0; i < PICTURES; i++)
	{
		g_positions[i] = Mat3x2::create_transform(
			Vec2(
				g_rng.real(-640.0f, 640.0f),
				g_rng.real(-320.0f, 320.0f)
			),
			g_rng.real(0.0f, Calc::TAU),
			Vec2::ONE * g_rng.real(0.1f, 0.7f),
			Vec2::ZERO
		);
	}
}

void update()
{
	g_postprocess->set("u_time", (int)Time::frames);
}

void render()
{
	g_framebuffer->clear(Colour(20, 200, 10));
	for (int i = 0; i < PICTURES; i++)
	{
		g_batch.push_matrix(g_positions[i]);
		g_batch.render_texture(g_texture);
		g_batch.pop_matrix();
	}
	g_batch.render(Mat4x4::create_orthographic(WINDOW_WIDTH, WINDOW_HEIGHT, 0.01f, 10000.0f), g_framebuffer);

	App::clear(Colour(0, 190, 170));
	g_batch.push_matrix(Mat3x2::create_transform(Vec2(-640, 360), 0.0f, Vec2(1.0f, -1.0f), Vec2::ZERO));
	g_batch.push_shader(g_postprocess);
	{
		g_batch.render_texture(g_framebuffer->attachment(0));
	}
	g_batch.pop_shader();
	g_batch.pop_matrix();
	g_batch.render(Mat4x4::create_orthographic(WINDOW_WIDTH, WINDOW_HEIGHT, 0.01f, 10000.0f));
}

int main(void)
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "framebuffer demo";
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
