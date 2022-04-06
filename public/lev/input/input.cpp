#include <lev/input/input.h>
#include <lev/core/app.h>

using namespace lev;

bool Input::init()
{
	return true;
}

void Input::destroy()
{
}

void Input::update()
{
	m_kb_prev = m_kb;
	m_mouse_prev = m_mouse;

	mem::set(m_kb.text, 0, LEV_MAX_TEXT_INPUT);
	m_mouse.wheel = Float2::zero();
}

void Input::on_mouse_move(float x, float y)
{
	m_mouse.position = Vec2F(x, y);

	m_mouse.draw_position = Vec2F(
		(x / App::inst()->window_width()) * App::inst()->draw_width(),
		(y / App::inst()->window_height()) * App::inst()->draw_height()
	);
}

void Input::on_mouse_screen_move(float x, float y)
{
	m_mouse.screen_position = Vec2F(x, y);
}

void Input::on_mouse_down(int button)
{
	m_mouse.down[button] = true;
}

void Input::on_mouse_up(int button)
{
	m_mouse.down[button] = false;
}

void Input::on_mouse_wheel(float x, float y)
{
	m_mouse.wheel = Float2(x, y);
}

void Input::on_key_down(int key)
{
	m_kb.down[key] = true;
}

void Input::on_key_up(int key)
{
	m_kb.down[key] = false;
}

void Input::on_text_utf8(const char* text)
{
	str::cncat(m_kb.text, text, LEV_MAX_TEXT_INPUT);
}

bool Input::down_mb(int mb)
{
	return m_mouse.down[mb];
}

bool Input::down_key(int key)
{
	return m_kb.down[key];
}

bool Input::released_mb(int mb)
{
	return !m_mouse.down[mb] && m_mouse_prev.down[mb];
}

bool Input::released_key(int key)
{
	return !m_kb.down[key] && m_kb_prev.down[key];
}

bool Input::pressed_mb(int mb)
{
	return m_mouse.down[mb] && !m_mouse_prev.down[mb];
}

bool Input::pressed_key(int key)
{
	return m_kb.down[key] && !m_kb_prev.down[key];
}

bool Input::ctrl()
{
	return down_key(KEY_LEFT_CONTROL) || down_key(KEY_RIGHT_CONTROL);
}

bool Input::shift()
{
	return down_key(KEY_LEFT_SHIFT) || down_key(KEY_RIGHT_SHIFT);
}

bool Input::alt()
{
	return down_key(KEY_LEFT_ALT) || down_key(KEY_RIGHT_ALT);
}

const char* Input::text()
{
	return m_kb.text;
}

Vec2F Input::mouse_screen_pos()
{
	return m_mouse.screen_position;
}

Vec2F Input::mouse_draw_pos()
{
	return m_mouse.draw_position;
}

Vec2F Input::mouse_position()
{
	return m_mouse.position;
}

Float2 Input::mouse_wheel()
{
	return m_mouse.wheel;
}

Float2 Input::mouse_wheel_change()
{
	return m_mouse.wheel - m_mouse_prev.wheel;
}
