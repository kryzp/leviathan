#include <lev/core/app.h>
#include <lev/input/input.h>
#include <lev/math/range.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/assets/asset_mgr.h>

using namespace lev;

#define PARTICLE_COUNT 25000
#define SCALE 1
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SIM_WIDTH (WINDOW_WIDTH / SCALE)
#define SIM_HEIGHT (WINDOW_HEIGHT / SCALE)

struct Particle
{
	float x, y;
	float ang;
};

int main()
{
	SpriteBatch batch;
	Range direction(0, Calc::TAU);

	Ref<Shader> compute_shader_part;
	Ref<Shader> compute_shader_post;
	Ref<Shader> shader_colourize;

	Ref<Texture> trail_map;
	Ref<Font> nokiafc;

	Ref<ShaderBuffer> particle_buf;
	Particle* particles = new Particle[PARTICLE_COUNT];
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		particles[i] = {
			.x = SIM_WIDTH / 2.0f,
			.y = SIM_HEIGHT / 2.0f,
			.ang = direction.random()
		};
	}

	Config conf;
	{
		conf.name = "ah yes: p a r t i c l e";

		conf.width = WINDOW_WIDTH;
		conf.height = WINDOW_HEIGHT;

		conf.target_fps = 144;

		conf.on_init = [&]()
		{
			trail_map = Texture::create(SIM_WIDTH, SIM_HEIGHT, TEXTURE_FORMAT_RGBA, TEXTURE_TYPE_UNSIGNED_BYTE, nullptr);

			particle_buf = ShaderBuffer::create(sizeof(Particle) * PARTICLE_COUNT);
			particle_buf->set(particles);

			nokiafc = AssetMgr::inst()->load<Font>(
				"nokia",
				FontLoadData(20, "D:\\_PROJECTS\\leviathan\\testing\\res\\fonts\\nokiafc22.ttf")
			);

			compute_shader_part = AssetMgr::inst()->load<Shader>(
				"compute_particle",
				ShaderLoadData("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\particle.comp")
			);

			compute_shader_post = AssetMgr::inst()->load<Shader>(
				"compute_post",
				ShaderLoadData("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\post.comp")
			);

			shader_colourize = AssetMgr::inst()->load<Shader>(
				"colourize",
				ShaderLoadData(
					"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\colourize.vert",
					"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\colourize.frag"
				)
			);
		};

		conf.on_update = [&]()
		{
			trail_map->bind_image(0);

			compute_shader_post->use()
				.dispatch_compute(SIM_WIDTH, SIM_HEIGHT, 1)
				.wait_compute();

			compute_shader_part->use()
				.set("u_delta_time", Time::delta)
				.set("u_mouse_pos", Input::inst()->mouse_draw_pos())
				.set("u_mouse_pressed", Input::inst()->down_mb(MB_LEFT))
				.set_buffer(particle_buf, 1)
				.dispatch_compute(PARTICLE_COUNT, 1, 1)
				.wait_compute();
		};

		conf.on_render = [&]()
		{
			App::inst()->clear();

			batch.push_matrix(Mat3x2::create_scale(SCALE));
			batch.push_shader(shader_colourize);
			batch.push_texture(trail_map);
			batch.pop_shader();
			batch.pop_matrix();
			
			batch.render();
		};
	}
	App::inst()->start(conf);

	delete[] particles;

	return 0;
}
