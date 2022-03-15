#include <leviathan.h>
#include "../../ext/physics/collider.h"
#include <iostream>

using namespace lev;

// really terrible code.
// its so so horrible.
// ...
// i got frustrated halfway through and started throwing stuff together in a messy way basically

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define BLUR_SCALE 4.0f

gfx::SpriteBatch g_batch;
Ref<gfx::Shader> g_shd_crt;
Ref<gfx::Shader> g_shd_bloom;
Ref<gfx::Shader> g_shd_gaussian;
Ref<gfx::Framebuffer> g_fbf;
Ref<gfx::Framebuffer> g_fbf2;
Ref<gfx::Framebuffer> g_fbf3Downscale;
Ref<gfx::Framebuffer> g_fbf3V;
Ref<gfx::Framebuffer> g_fbf3H;
Ref<gfx::Texture> g_tex;
Ref<gfx::Texture> g_tex_box;
phys::Collider2D g_col;
phys::Collider2D g_col_box;
Transform g_transform;
Vec2 g_velocity;
float g_rot_intermediate;
float g_curr_rotation;
float g_tgt_rotation;

void init()
{
	gfx::TextureFormat fbf2_attachments[2] = {
		gfx::TextureFormat::RGBA, // normal crt output
		gfx::TextureFormat::RGBA  // crt output only bright colours
	};

	g_fbf           = gfx::Framebuffer::create(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4);
	g_fbf2          = gfx::Framebuffer::create(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, fbf2_attachments, 2);
	g_fbf3Downscale = gfx::Framebuffer::create(WINDOW_WIDTH / BLUR_SCALE, WINDOW_HEIGHT / BLUR_SCALE);
	g_fbf3V         = gfx::Framebuffer::create(WINDOW_WIDTH / BLUR_SCALE, WINDOW_HEIGHT / BLUR_SCALE);
	g_fbf3H         = gfx::Framebuffer::create(WINDOW_WIDTH, WINDOW_HEIGHT);

	g_tex = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png");
	g_tex_box = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p1.png");

	g_shd_crt      = gfx::Shader::create("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\generic.vert", "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\crt.frag");
	g_shd_bloom    = gfx::Shader::create("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\generic.vert", "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\bloom.frag");
	g_shd_gaussian = gfx::Shader::create("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\generic.vert", "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\gaussian.frag");
	
	g_transform.origin(128, 128);
	g_col = phys::Collider2D(0, 0, 256, 256);
	g_col.parent = &g_transform;

	g_col_box = phys::Collider2D(Polygon({
		Vec2(-128, -128),
		Vec2(128, -128),
		Vec2(0, 128)
	}));
	g_col_box.transform.position(1280/2, 720/2);
}

void update()
{
	auto modifier = Input::shift()?0.35f:Input::ctrl()?1.75f:1.0f;
	if (Input::down(Key::Q)) g_tgt_rotation -= 5.0f * modifier * Time::delta;
	if (Input::down(Key::E)) g_tgt_rotation += 5.0f * modifier * Time::delta;

	g_velocity = (Input::mouse_position() - g_transform.position()) * 0.25f;
	g_transform.move(g_velocity);

	g_curr_rotation += Calc::spring(g_curr_rotation, g_tgt_rotation, 10.0f, 0.5f, g_rot_intermediate);
	g_transform.rotation(g_curr_rotation);

	Vec2 pushout;
	if (g_col.overlaps(g_col_box, &pushout))
		g_transform.move(pushout);
}

void render()
{
	// render scene to g_fbf
	{
		g_fbf->clear();
		
		gfx::Material mat;
		mat.shader = nullptr;
		mat.textures = { g_tex_box };
		mat.samplers = { gfx::TextureSampler::pixel() };
		
		g_batch.push_matrix(Mat3x2::create_scale(0.25f));
		g_batch.push_material(mat);
		{
			g_batch.push_matrix(Mat3x2::create_transform(Vec2(1280/2, 720/2), 0.0f, Vec2::one(), Vec2(128, 128)));
			g_batch.quad();
			g_batch.pop_matrix();
			
			g_batch.peek_material().textures[0] = g_tex;

			g_batch.push_matrix(g_transform.matrix());
			g_batch.quad();
			g_batch.pop_matrix();
		}
		g_batch.pop_material();
		g_batch.pop_matrix();

		g_batch.render(g_fbf);
	}

	// render g_fbf to g_fbf2 with a crt shader and extract bright colours
	{
		g_fbf2->clear();

		gfx::Material mat;
		mat.shader = g_shd_crt;
		mat.textures = { g_fbf->attachment(0) };
		mat.samplers = { gfx::TextureSampler::pixel() };

		g_batch.push_material(mat);
		{
			g_batch.peek_shader()->use().set("u_time", Time::milliseconds/1000.0f);
			g_batch.quad();
		}
		g_batch.pop_material();

		g_batch.render(g_fbf2);
	}

	// apply gaussian blur to g_fbf2's bright colours and render that to g_fbf3
	{
		gfx::Material mat;
		mat.shader = nullptr;
		mat.textures = { g_fbf2->attachment(1) };
		mat.samplers = { gfx::TextureSampler::linear() };

		g_batch.push_material(mat);

		g_fbf3Downscale->clear();

		g_batch.push_matrix(Mat3x2::create_scale(1.0f/BLUR_SCALE));
		{
			g_batch.quad();
		}
		g_batch.pop_matrix();
		g_batch.render(g_fbf3Downscale);

		g_batch.peek_material().shader = g_shd_gaussian;
		g_batch.peek_material().textures[0] = g_fbf3Downscale->attachment(0);
		
		g_fbf3V->clear();
		{
			g_batch.peek_shader()->use().set("u_horizontal", false);
			g_batch.quad();
		}
		g_batch.render(g_fbf3V);

		g_batch.peek_material().textures[0] = g_fbf3V->attachment(0);

		g_batch.push_matrix(Mat3x2::create_scale(4.0f * BLUR_SCALE));
		g_fbf3H->clear();
		{
			g_batch.peek_shader()->use().set("u_horizontal", true);
			g_batch.quad();
		}
		g_batch.pop_matrix();

		g_batch.pop_material();
		g_batch.render(g_fbf3H);
	}

	// finally, render g_fbf2 and g_fbf3[VH] with bloom shader to the world
	{
		App::clear(0x0D0D0DFF);

		gfx::Material mat;
		mat.shader = g_shd_bloom;
		mat.textures = { g_fbf2->attachment(0), g_fbf3H->attachment(0) };
		mat.samplers = { gfx::TextureSampler::linear(), gfx::TextureSampler::linear() };
		
		g_batch.push_matrix(Mat3x2::create_scale(4.0f));
		g_batch.push_material(mat);
		{
			g_batch.texture();
		}
		g_batch.pop_material();
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
