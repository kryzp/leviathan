#include <lev/core/app.h>
#include <lev/input/input.h>
#include <lev/math/range.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/containers/bitset.h>
#include <lev/assets/asset_mgr.h>

using namespace lv;

#define PARTICLE_COUNT 300000
#define SCALE 1
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SIM_WIDTH (WINDOW_WIDTH / SCALE)
#define SIM_HEIGHT (WINDOW_HEIGHT / SCALE)

struct Particle
{
	// float = 4 bytes
	// hence, no padding is needed

	float x, y;
	float ang;
};

int main()
{
	SpriteBatch batch;
	Range direction(0, calc::TAU);

	AssetMgr assets;

	Ref<Shader> compute_shader_part;
	Ref<Shader> compute_shader_post;
	Ref<Shader> shader_colourize;

	Ref<Texture> trail_map;
	Ref<Font> nokiafc;

	Ref<ShaderBuffer> particle_buf;
	auto* particles = new Particle[PARTICLE_COUNT];
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
			trail_map = Texture::create(SIM_WIDTH, SIM_HEIGHT, TEX_FMT_RGBA, TEX_I_FMT_RGBA32F, TEX_TYPE_UNSIGNED_BYTE, nullptr);

			particle_buf = ShaderBuffer::create(sizeof(Particle) * PARTICLE_COUNT);
			particle_buf->set(particles);

			nokiafc = assets.load<Font>("res\\fonts\\nokiafc.levfont");
			compute_shader_part = assets.load<Shader>("res\\shaders\\particle.levshader");
			compute_shader_post = assets.load<Shader>("res\\shaders\\post.levshader");
			shader_colourize = assets.load<Shader>("res\\shaders\\colourize.levshader");

			Image img(100, 100);
			img.paint([&](u32 x, u32 y) -> Colour
			{
				return Colour(
					(float)x * 2.55f,
					(float)y * 2.55f,
					10,
					255
				);
			});

			img.save_png("lol.png");
		};

		conf.on_update = [&]()
		{
			trail_map->bind_image(0);

			compute_shader_post->use()
				.dispatch_compute(SIM_WIDTH, SIM_HEIGHT, 1)
				.wait_compute();

			compute_shader_part->use()
				.set("u_delta_time", time::delta)
				.set("u_mouse_pos", Input::inst()->mouse_draw_pos())
				.set("u_mouse_pressed", Input::inst()->down_mb(MB_LEFT))
				.set_buffer(particle_buf, 1)
				.dispatch_compute(PARTICLE_COUNT, 1, 1)
				.wait_compute();
		};

		conf.on_render = [&]()
		{
			App::clear();

			batch.push_matrix(Mat3x2::create_scale(SCALE));
			batch.set_shader(shader_colourize);
			batch.push_texture(trail_map);
			batch.reset_shader();
			batch.pop_matrix();
			
			batch.render();
		};
	}
	App::inst()->start(conf);

	delete[] particles;

	return 0;
}
