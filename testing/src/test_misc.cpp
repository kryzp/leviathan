#include <lev/core/app.h>
#include <lev/math/rng.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/assets/asset_mgr.h>

using namespace lev;

#define PARTICLE_COUNT 250
#define SCALE 4
#define WINDOW_WIDTH (1280 / SCALE)
#define WINDOW_HEIGHT (720 / SCALE)

struct Particle
{
	float x;
	float y;
	float ang;
};

int main()
{
	SpriteBatch batch;
	Rng rng;

	Ref<Shader> compute_shader_part;
	Ref<Shader> compute_shader_fade;
	Ref<Shader> compute_shader_blur;

	Ref<Texture> tex;
	Ref<Font> nokiafc;

	Particle* particles = new Particle[PARTICLE_COUNT];

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		particles[i] = {
			.x = WINDOW_WIDTH / 2.0f,
			.y = WINDOW_HEIGHT / 2.0f,
			.ang = rng.real(0, Calc::TAU)
		};
	}

	Ref<ShaderBuffer> particle_buf;

	Config conf;
	{
		conf.name = "ah yes: p a r t i c l e";
		//conf.vsync = false;

		conf.width = WINDOW_WIDTH * SCALE;
		conf.height = WINDOW_HEIGHT * SCALE;

		conf.on_init = [&]()
		{
			tex = Texture::create(WINDOW_WIDTH, WINDOW_HEIGHT, TEXTURE_FORMAT_RGBA, TEXTURE_TYPE_UNSIGNED_BYTE, nullptr);

			nokiafc = AssetMgr::inst().load<Font>("nokia", FontLoadData {
				.size = 20,
				.path = "D:\\_PROJECTS\\leviathan\\testing\\res\\fonts\\nokiafc22.ttf"
			});

			particle_buf = ShaderBuffer::create(sizeof(Particle) * PARTICLE_COUNT);
			particle_buf->set(particles);

			compute_shader_part = AssetMgr::inst().load<Shader>("compute_particle", ShaderLoadData {
				.type = SHADER_TYPE_COMPUTE,
				.is_source = false,
				.compute = "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\particle.comp"
			});

			compute_shader_fade = AssetMgr::inst().load<Shader>("compute_fade", ShaderLoadData {
				.type = SHADER_TYPE_COMPUTE,
				.is_source = false,
				.compute = "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\fade.comp"
			});

			compute_shader_blur = AssetMgr::inst().load<Shader>("compute_blur", ShaderLoadData {
				.type = SHADER_TYPE_COMPUTE,
				.is_source = false,
				.compute = "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\blur.comp"
			});
		};

		conf.on_render = [&]()
		{
			App::clear();

			tex->bind_image(0);

			compute_shader_blur->use()
				.dispatch_compute(WINDOW_WIDTH, WINDOW_HEIGHT, 1)
				.wait_compute();

			compute_shader_fade->use()
				.dispatch_compute(WINDOW_WIDTH, WINDOW_HEIGHT, 1)
				.wait_compute();

			compute_shader_part->use()
				.set("u_delta_time", Time::delta)
				.set_buffer(particle_buf, 1)
				.dispatch_compute(PARTICLE_COUNT, 1, 1)
				.wait_compute();

			batch.push_matrix(Mat3x2::create_scale(Vec2F::one() * SCALE));
			batch.push_texture(tex);
			batch.pop_matrix();

			batch.push_matrix(Mat3x2::create_transform(Vec2F(10, 40), 0.0f, Vec2F::one() * 2.0f, Vec2F::zero()));
			batch.push_text(("fps: " + std::to_string(App::fps())).c_str(), *nokiafc, TEXT_ALIGN_LEFT, Colour::white());
			batch.pop_matrix();

			batch.render();
		};
	}
	App::start(conf);

	delete[] particles;

	return 0;
}
