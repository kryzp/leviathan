#include <lev/core/app.h>
#include <lev/math/rng.h>
#include <lev/graphics/sprite_batch.h>

using namespace lev;

#define PARTICLE_COUNT 1000000

struct Particle
{
	float x, y;
	float ang;
};

int main()
{
	SpriteBatch batch;
	Rng rng;

	Ref<Shader> compute_shader;
	Ref<Shader> compute_shader_fade;
	Ref<Texture> tex;
	Ref<Font> nokiafc;

	Particle* particles = new Particle[PARTICLE_COUNT];

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		particles[i] = {
			.x = 1280.0f / 2.0f,
			.y = 720.0f / 2.0f,
			.ang = rng.real(0, Calc::TAU)
		};
	}

	Ref<ShaderBuffer> particle_buf;

	Config conf;
	{
		conf.name = "ah yes: p a r t i c l e";
		//conf.vsync = false;

		conf.on_init = [&]()
		{
			tex = Texture::create(App::window_width(), App::window_height(), lev::TEXTURE_FORMAT_RGBA, lev::TEXTURE_TYPE_UNSIGNED_BYTE, nullptr);
			nokiafc = create_ref<Font>(20, "D:\\_PROJECTS\\leviathan\\testing\\res\\fonts\\nokiafc22.ttf");
			compute_shader = Shader::create_compute("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\particle.comp");
			compute_shader_fade = Shader::create_compute("D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\fade.comp");
			particle_buf = ShaderBuffer::create(particles, sizeof(Particle) * PARTICLE_COUNT);
		};

		conf.on_render = [&]()
		{
			App::clear();

			tex->bind_image(0);

			compute_shader_fade->use()
				.dispatch_compute(tex->width(), tex->height(), 1)
				.wait_compute();

			compute_shader->use()
				.set("delta_time", Time::delta)
				.set_buffer(particle_buf, 1)
				.dispatch_compute(PARTICLE_COUNT, 1, 1)
				.wait_compute();

			batch.push_texture(tex);

			batch.push_matrix(Mat3x2::create_transform(Vec2F(10, 40), 0.0f, Vec2F::one() * 2.0f, Vec2F::zero()));
			batch.push_text((std::string("fps: ") + std::to_string(App::fps())).c_str(), *nokiafc, TEXT_ALIGN_LEFT, Colour::black());
			batch.pop_matrix();

			batch.render();
		};
	}
	App::start(conf);
	return 0;
}
