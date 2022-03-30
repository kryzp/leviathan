#include <lev/input/input.h>
#include <lev/core/app.h>

using namespace lev;

namespace
{
	KeyboardState g_kb;
	KeyboardState g_kb_prev;

	MouseState g_mouse;
	MouseState g_mouse_prev;
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
	g_kb_prev = g_kb;
	g_mouse_prev = g_mouse;

	MemUtil::set(g_kb.text, 0, Input::MAX_TEXT_INPUT);

	g_mouse.wheel = Float2::zero();
}

void Input::on_mouse_move(float x, float y)
{
	g_mouse.position = Vec2F(x, y);

	g_mouse.draw_position = Vec2F(
		(x / App::window_width()) * App::draw_width(),
		(y / App::window_height()) * App::draw_height()
	);
}

void Input::on_mouse_screen_move(float x, float y)
{
	g_mouse.screen_position = Vec2F(x, y);
}

void Input::on_mouse_down(MouseButton button)
{
	g_mouse.down[button] = true;
}

void Input::on_mouse_up(MouseButton button)
{
	g_mouse.down[button] = false;
}

void Input::on_mouse_wheel(float x, float y)
{
	g_mouse.wheel = Float2(x, y);
}

void Input::on_key_down(Key key)
{
	g_kb.down[key] = true;
}

void Input::on_key_up(Key key)
{
	g_kb.down[key] = false;
}

void Input::on_text_utf8(const char* text)
{
	StrUtil::cncat(g_kb.text, text, Input::MAX_TEXT_INPUT);
}

bool Input::down(MouseButton mb)
{
	return g_mouse.down[mb];
}

bool Input::down(Key key)
{
	return g_kb.down[key];
}

bool Input::released(MouseButton mb)
{
	return !g_mouse.down[mb] && g_mouse_prev.down[mb];
}

bool Input::released(Key key)
{
	return !g_kb.down[key] && g_kb_prev.down[key];
}

bool Input::pressed(MouseButton mb)
{
	return g_mouse.down[mb] && !g_mouse_prev.down[mb];
}

bool Input::pressed(Key key)
{
	return g_kb.down[key] && !g_kb_prev.down[key];
}

bool Input::ctrl()
{
	return down(KEY_LEFT_CONTROL) || down(KEY_RIGHT_CONTROL);
}

bool Input::shift()
{
	return down(KEY_LEFT_SHIFT) || down(KEY_RIGHT_SHIFT);
}

bool Input::alt()
{
	return down(KEY_LEFT_ALT) || down(KEY_RIGHT_ALT);
}

const char* Input::text()
{
	return g_kb.text;
}

Vec2F Input::mouse_screen_pos()
{
	return g_mouse.screen_position;
}

Vec2F Input::mouse_draw_pos()
{
	return g_mouse.draw_position;
}

Vec2F Input::mouse_position()
{
	return g_mouse.position;
}

Float2 Input::mouse_wheel()
{
	return g_mouse.wheel;
}

Float2 Input::mouse_wheel_change()
{
	return g_mouse_prev.wheel - g_mouse.wheel;
}
