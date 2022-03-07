#include <leviathan.h>
#include <lev/ui/components/texture_component.h>

using namespace lev;

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

Ref<gfx::Texture> tex0;

gfx::SpriteBatch batch;
ui::UIComponent ui_container;

void init()
{
	batch.initialize();

	tex0 = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png");

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
	ui_container.update();
}

void render()
{
	App::clear(Colour(0, 190, 170));
	ui_container.render(batch);
	batch.render(Mat4x4::create_orthographic(1280.0f, 720.0f, 0.0f, 1000.0f));
}

int main(void)
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "ui demo";
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
