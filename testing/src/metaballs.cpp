#include <lev/core/app.h>
#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/math/rand.h>
#include <lev/math/circle.h>
#include <lev/math/vec2.h>

#define BALL_COUNT 16
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct Ball
{
	lv::Circle shape;
	lv::Vec2F velocity;
};

static lv::SpriteBatch g_batch;
static lv::AssetMgr g_assets;
static lv::Ref<lv::RenderTarget> g_target;
static lv::Ref<lv::Shader> g_metaball_shader;
static Ball g_balls[BALL_COUNT];

static void init_balls()
{
	for (int i = 0; i < BALL_COUNT; i++)
	{
		auto& b = g_balls[i];

		b.shape.position = lv::Vec2F(
			lv::Rand<>::inst()->real(0.0f, WINDOW_WIDTH),
			lv::Rand<>::inst()->real(0.0f, WINDOW_HEIGHT)
		);

		b.shape.radius = lv::Rand<>::inst()->real(100.0f, 200.0f);

		b.velocity = lv::Vec2F::random_unit() * lv::Rand<>::inst()->real(20.0f, 40.0f);
	}
}

static void init_render_stuff()
{
	g_target          = lv::RenderTarget::create(WINDOW_WIDTH, WINDOW_HEIGHT);
	g_metaball_shader = g_assets.load<lv::Shader>("res\\shaders\\metaballs_shader.levshader");
}

static void init()
{
	init_render_stuff();
	init_balls();
}

static void update()
{
	for (auto& b : g_balls)
		b.shape.position += b.velocity * lv::time::delta;
}

static void render()
{
	// render to target
	{
		g_target->clear();

		for (auto& b : g_balls)
			g_batch.push_circle_col(b.shape, lv::Colour::white(), lv::Colour::empty());

		g_batch.render(g_target);
	}

	// render to screen
	{
		lv::App::clear();
		g_batch.set_shader(g_metaball_shader);
		g_batch.push_texture(g_target->attachment(0));
		g_batch.reset_shader();

		g_batch.push_line_col(lv::Line(lv::Vec2F(50, 50), 45.0f * lv::calc::DEG2RAD, 50.0f), 10.0f,  lv::Colour::red(), lv::Colour::blue());

		g_batch.render();
	}
}

int main()
{
	lv::Config conf;
	{
		conf.name = "meta's balls";
		conf.width = WINDOW_WIDTH;
		conf.height = WINDOW_HEIGHT;
		conf.resizable = false;
		conf.target_fps = 60;
		conf.vsync = true;
		conf.max_updates = 5;
		conf.cursor_visible = true;
		conf.on_init = init;
		conf.on_update = update;
		conf.on_render = render;
	}
	lv::App::inst()->start(conf);

	return 0;
}
