#include <leviathan.h>
#include "../../ext/physics/collider.h"
#include <iostream>

/*
playing around with random stuff
*/

using namespace lev;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

gfx::SpriteBatch g_batch;
Ref<gfx::Shader> g_postprocess;

Ref<gfx::Texture> g_tex;
phys::Collider2D g_col;
Transform g_transform;

Ref<gfx::Texture> g_tex_box;
phys::Collider2D g_col_box;

Ref<gfx::Framebuffer> g_fbf;

float g_curr_rotation;
float g_tgt_rotation;
float g_rot_time;

void init()
{
	g_fbf = gfx::Framebuffer::create(WINDOW_WIDTH, WINDOW_HEIGHT);

	g_transform.position(-500, 0);
	g_transform.origin(128, 128);
	g_transform.scale(1.0f, 1.0f);
	g_transform.rotation(0.0f);

	g_tex = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p1.png");
	g_tex_box = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");
	
	g_postprocess = gfx::Shader::create(
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.vert",
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.frag"
	);

	g_col = phys::Collider2D(0, 0, 256, 256);
	g_col.parent = &g_transform;

	g_col_box = phys::Collider2D(-128, -128, 256, 256);
}

void update()
{
	auto modifier = Input::shift()?0.35f:Input::ctrl()?1.75f:1.0f;

	if (Input::down(Key::A)) { g_transform.movex( -500.0f * modifier * Time::delta); }
	if (Input::down(Key::D)) { g_transform.movex(  500.0f * modifier * Time::delta); }
	if (Input::down(Key::W)) { g_transform.movey( -500.0f * modifier * Time::delta); }
	if (Input::down(Key::S)) { g_transform.movey(  500.0f * modifier * Time::delta); }
	if (Input::down(Key::Q)) { g_tgt_rotation +=  -2.0f   * modifier * Time::delta ; }
	if (Input::down(Key::E)) { g_tgt_rotation +=   2.0f   * modifier * Time::delta ; }

	g_rot_time++;
	g_curr_rotation = Calc::lerp(g_curr_rotation, g_tgt_rotation, 0.05f);

	g_transform.rotation(g_curr_rotation);

	Vec2 pushout;
	if (g_col.overlaps(g_col_box, &pushout))
		g_transform.move(pushout);
}

void render()
{
	g_fbf->clear(Colour(60, 190, 30));
	{
		g_batch.push_matrix(g_transform.matrix());
		g_batch.texture(g_tex);
		g_batch.pop_matrix();

		g_batch.push_matrix(Mat3x2::create_transform(Vec2::ZERO, 0.0f, Vec2::ONE, Vec2(128, 128)));
		g_batch.texture(g_tex_box);
		g_batch.pop_matrix();
	}
	g_batch.render(g_fbf);

	App::clear(Colour(60, 190, 30));
	{
		g_batch.push_matrix(Mat3x2::create_transform(Vec2(-1280/2, -720/2), 0.0f, Vec2::ONE, Vec2::ZERO));
		g_batch.push_shader(g_postprocess);
		g_batch.texture(g_fbf->attachment(0));
		g_batch.pop_shader();
		g_batch.pop_matrix();
	}
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
	conf.vsync = true;
	conf.on_init = init;
	conf.on_update = update;
	conf.on_render = render;

	App::start(conf);

	return 0;
}
