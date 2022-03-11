#include <lev/input/input.h>
#include <lev/core/app.h>

using namespace lev;

namespace
{
	KeyboardState g_kb;
	KeyboardState g_prev_kb;

	MouseState g_mouse;
	MouseState g_prev_mouse;
}

bool Input::init()
{
	return true;
}

void Input::destroy()
{
}

void Input::update()
{
	g_prev_kb = g_kb;
	g_prev_mouse = g_mouse;

	for (int i = 0; i < (int)Key::MAX; i++)
		g_kb.pressed[i] = g_kb.released[i] = false;
	MemUtil::set_zero(g_kb.text, Input::MAX_TEXT_INPUT);

	for (int i = 0; i < (int)MouseButton::MAX; i++)
		g_mouse.released[i] = g_mouse.pressed[i] = false;
	g_mouse.wheel = Float2::ZERO;
}

void Input::on_mouse_move(float x, float y)
{
	g_mouse.position = Vec2(x, y);

	g_mouse.draw_position = Vec2(
		x/App::window_width() * App::draw_width(),
		y/App::window_height() * App::draw_height()
	);
}

void Input::on_mouse_screen_move(float x, float y)
{
	g_mouse.screen_position = Vec2(x, y);
}

void Input::on_mouse_down(MouseButton button)
{
	g_mouse.down[(int)button] = true;
}

void Input::on_mouse_up(MouseButton button)
{
	g_mouse.down[(int)button] = false;
	g_mouse.released[(int)button] = true;
}

void Input::on_mouse_wheel(float x, float y)
{
	g_mouse.wheel = Float2(x, y);
}

void Input::on_key_down(Key key)
{
	g_kb.down[(int)key] = true;
}

void Input::on_key_up(Key key)
{
	g_kb.down[(int)key] = false;
	g_kb.released[(int)key] = true;
}

void Input::on_text_utf8(const char* text)
{
	strncat(g_kb.text, text, Input::MAX_TEXT_INPUT);
}

bool Input::down(MouseButton mb)
{
	return g_mouse.down[(int)mb];
}

bool Input::down(Key key)
{
	return g_kb.down[(int)key];
}

bool Input::released(MouseButton mb)
{
	return g_mouse.released[(int)mb];
}

bool Input::released(Key key)
{
	return g_mouse.released[(int)key];
}

bool Input::pressed(MouseButton mb)
{
	return g_mouse.pressed[(int)mb];
}

bool Input::pressed(Key key)
{
	return g_kb.pressed[(int)key];
}

bool Input::ctrl()
{
	return g_kb.down[(int)Key::LEFT_CONTROL] || g_kb.down[(int)Key::RIGHT_CONTROL];
}

bool Input::shift()
{
	return g_kb.down[(int)Key::LEFT_SHIFT] || g_kb.down[(int)Key::RIGHT_SHIFT];
}

bool Input::alt()
{
	return g_kb.down[(int)Key::LEFT_ALT] || g_kb.down[(int)Key::RIGHT_ALT];
}

const char* Input::text()
{
	return g_kb.text;
}

Vec2 Input::mouse_screen_pos()
{
	return g_mouse.screen_position;
}

Vec2 Input::mouse_draw_pos()
{
	return g_mouse.draw_position;
}

Vec2 Input::mouse_position()
{
	return g_mouse.position;
}

Float2 Input::mouse_wheel()
{
	return g_mouse.wheel;
}

Float2 Input::mouse_wheel_change()
{
	return g_prev_mouse.wheel - g_mouse.wheel;
}
