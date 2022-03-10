#include <leviathan.h>

using namespace lev;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

gfx::SpriteBatch g_batch;
Ref<gfx::Shader> g_postprocess;
Ref<gfx::Texture> g_tex;
Ref<gfx::Framebuffer> g_fbf;

Vec2 position;
float rotation;

void init()
{
	g_fbf = gfx::Framebuffer::create(1280, 720);
	g_tex = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");
	g_postprocess = gfx::Shader::create(
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.vert",
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.frag"
	);
}

void update()
{
	auto modifier = Input::shift()?0.35f:Input::ctrl()?1.75f:1.0f;

	if (Input::down(Key::A)) position.x -= 500.0f * modifier * Time::delta;
	if (Input::down(Key::D)) position.x += 500.0f * modifier * Time::delta;
	if (Input::down(Key::W)) position.y -= 500.0f * modifier * Time::delta;
	if (Input::down(Key::S)) position.y += 500.0f * modifier * Time::delta;
	if (Input::down(Key::Q)) rotation   -= 2.0f   * modifier * Time::delta;
	if (Input::down(Key::E)) rotation   += 2.0f   * modifier * Time::delta;
}

void render()
{
	g_fbf->clear(Colour(60, 190, 30));
	g_batch.push_matrix(Mat3x2::create_transform(position, rotation, Vec2::ONE, Vec2(128, 128)));
	g_batch.texture(g_tex);
	g_batch.pop_matrix();
	g_batch.render(g_fbf);

	App::clear(Colour(60, 190, 30));
	g_batch.push_matrix(Mat3x2::create_transform(Vec2(-1280/2, -720/2), 0.0f, Vec2::ONE, Vec2::ZERO));
	g_batch.push_shader(g_postprocess);
	g_batch.texture(g_fbf->attachment(0));
	g_batch.pop_shader();
	g_batch.pop_matrix();
	g_batch.render();
}

int main(void)
{
	AppConfig conf;
	conf.name = "yes";
	conf.width = WINDOW_WIDTH;
	conf.height = WINDOW_HEIGHT;
	conf.target_framerate = 60;
	conf.resizable = true;
	conf.on_init = init;
	conf.on_update = update;
	conf.on_render = render;

	App::start(conf);

	return 0;
}
